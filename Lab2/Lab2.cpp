#include <iostream>
#include <iomanip>
#include <vector>
#include <ranges>
#include <cstddef>
#include <array>
#include <bitset>

#include <boost/intrusive/slist.hpp>
#include <boost/intrusive/options.hpp>

using namespace boost::intrusive;


constexpr auto PAGESIZE = 4U;

constexpr auto PAGECOUNT = 4U;

using Page =
std::array<int, PAGESIZE>;

struct Ram {
	struct InfoBlock {
		struct PageInfo : public slist_base_hook<cache_last<true>> {
			enum : bool {
				RAM = 0b0,
				HDD = 0b1,
			};
			bool
				R : 1 = 0,
				M : 1 = 0,
				Type : 1;
			std::size_t real{};
		};
		std::array<PageInfo, 2 * PAGECOUNT> table;
		slist<PageInfo, cache_last<true>> queue;
	}
	info;
	std::array<Page, PAGECOUNT> pages{};
};


struct Hdd {
	std::array<Page, PAGECOUNT> pages{};
};


class VirtualMemory {
	constexpr static int REFRESH_TICKS = 4;

	Ram ram;
	Hdd hdd;
	std::size_t ticks = 0;

	bool refresh() {
		return (++ticks %= (REFRESH_TICKS + 1)) == REFRESH_TICKS;
	}
	void clearRM() {
		for (auto& info : ram.info.table)
			if (info.Type == info.RAM)
				info.R = info.M = 0;
	}

	void memoryInteruption(Ram::InfoBlock::PageInfo& info) {
		while (ram.info.queue.front().R) {
			ram.info.queue.front().R = 0;
			ram.info.queue.shift_forward();
		}

		auto& oldest = ram.info.queue.front();
		ram.info.queue.pop_front();

		std::swap(hdd.pages[info.real], ram.pages[oldest.real]);

		oldest.Type = oldest.HDD;
		oldest.R = oldest.M = 0;

		info.Type = info.RAM;
		std::swap(info.real, oldest.real);

		ram.info.queue.push_back(info);
	}
public:
	VirtualMemory() {
		for (int i = 0; i < PAGECOUNT; ++i) {
			ram.info.table[i].Type = Ram::InfoBlock::PageInfo::RAM;
			ram.info.table[i].real = i;
			ram.info.table[PAGECOUNT + i].Type = Ram::InfoBlock::PageInfo::HDD;
			ram.info.table[PAGECOUNT + i].real = i;
		}
		for (auto& pageinfo : ram.info.table) {
			if (pageinfo.Type == pageinfo.RAM)
				ram.info.queue.push_back(pageinfo);
		}
	}

	Page read(std::size_t virtual_address) {
		if (refresh())
			clearRM();

		auto& info = ram.info.table[virtual_address];
		std::cout << "try page #" << virtual_address << '\n';
		if (info.Type == info.HDD) {
			std::cout << "memory miss\n";

			memoryInteruption(info);

			info.R = 1; info.M = 0;

			return ram.pages[info.real];
		}
		else {
			std::cout << "memory hit\n";
			info.R = 1;
			return ram.pages[info.real];
		}
	}

	void write(std::size_t virtual_address, std::size_t idx, int value) {
		if (refresh())
			clearRM();

		auto& info = ram.info.table[virtual_address];
		std::cout << "try page #" << virtual_address << '\n';
		if (info.Type == info.HDD) {
			std::cout << "memory miss\n";

			memoryInteruption(info);

			info.R = 0; info.M = 1;

			ram.pages[info.real][idx] = value;
		}
		else {
			std::cout << "memory hit\n";
			info.M = 1;
			ram.pages[info.real][idx] = value;
		}
	}

	void printInfo() {
		std::cout << "\n\ttable\n";
		std::cout << std::setw(4) << "id" << std::setw(7) << "where" << std::setw(6) << "real" << std::setw(4) << "val" << std::setw(4) << "RM" << '\n';

		for (int i = 0; i < ram.info.table.size(); ++i) {
			const auto& info = ram.info.table[i];
			std::cout << std::setw(4) << i << std::setw(7) << (info.Type ? " HDD " : " RAM ") << std::setw(6) <<
				info.real << std::setw(4) <<
				(info.Type ? hdd.pages[info.real][0] : ram.pages[info.real][0])
				<< std::setw(4) <<
				(info.R * 10 | (int)info.M) << '\n';
		}
		std::cout << "\tqueue: ";
		for (const auto& info : ram.info.queue)
			std::cout << info.real << ' ';
		std::cout << "\n\n";
	}
};



int main()
{
	VirtualMemory vm;
	vm.printInfo();


	while (1) {
		std::cout << "1 [page] [idx] to read | 2 [page] [idx] [value] to write\n";
		int choice, page, idx;
		std::cin >> choice >> page >> idx;
		switch (choice) {
		case 1:
			std::cout << vm.read(page, 0);
			vm.printInfo();
			break;
		}
		case 2:
			int value;
			std::cin >> value;
			vm.write(page, 0, value);
			vm.printInfo();
			break;
		default:
			std::cout << "bad input\n";
		}
	}
}
