#include <iostream>
#include <random>
#include <iomanip>
#include <vector>
#include <ranges>
#include <cstddef>
#include <array>
#include <bitset>

#include <boost/intrusive/slist.hpp>
#include <boost/intrusive/options.hpp>

using boost::intrusive::cache_last;

constexpr auto PAGESIZE = 4U;

constexpr auto PAGECOUNT = 4U;

using Page =
std::array<int, PAGESIZE>;

struct Ram {
	struct InfoBlock {
		struct PageInfo : public boost::intrusive::slist_base_hook<cache_last<true>> {
			enum : bool {
				RAM = false,
				HDD = true,
			};
			bool
				R : 1 = false,
				M : 1 = false,
				Type : 1 = RAM;
			std::size_t real{};
		};
		std::array<PageInfo, 2 * PAGECOUNT> table;
		boost::intrusive::slist<PageInfo, cache_last<true>> queue;
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
		return (++ticks %= REFRESH_TICKS) == 0;
	}
	void clearRM() {
		for (auto& info : ram.info.table)
			if (info.Type == info.RAM)
				info.R = info.M = false;
	}

	void memoryInterruption(Ram::InfoBlock::PageInfo& requested_page_info) {
		while (ram.info.queue.front().R) {
			ram.info.queue.front().R = false;
			ram.info.queue.shift_forward();
		}

		auto& oldest_page_info = ram.info.queue.front();
		ram.info.queue.pop_front();

		std::swap(hdd.pages[requested_page_info.real], ram.pages[oldest_page_info.real]);

		oldest_page_info.Type = oldest_page_info.HDD;
		oldest_page_info.R = oldest_page_info.M = false;
		requested_page_info.Type = requested_page_info.RAM;

		std::swap(requested_page_info.real, oldest_page_info.real);

		ram.info.queue.push_back(requested_page_info);
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

			memoryInterruption(info);

			info.R = true; info.M = false;

			return ram.pages[info.real];
		}
		else {
			std::cout << "memory hit\n";
			info.R = true;
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

			memoryInterruption(info);

			info.R = false; info.M = true;

			ram.pages[info.real][idx] = value;
		}
		else {
			std::cout << "memory hit\n";
			info.M = true;
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
		std::cout << "\nticks: " << ticks;
		std::cout << "\n\n";
	}
};


void autotest();
void ui();

int main()
{
	//autotest();
	ui();
}

auto getRandom(auto begin_inclusive, auto end_inclusive) {
	return [gen = std::random_device(),
		distr = std::uniform_int_distribution<decltype(begin_inclusive)>(begin_inclusive, end_inclusive)]() mutable {
		return distr(gen);
	};
}

int fuzzing() {
	VirtualMemory vm;

	auto getRndPage =
		getRandom(0, 2 * PAGECOUNT - 1);
	auto getRndIdx =
		getRandom(0, PAGESIZE - 1);
	auto getRndMove =
		getRandom(0, 1);
	auto getRndValue =
		getRandom(1, 999);

	try {
		for (int i = 0; i < 1'000; ++i) {
			const auto page = getRndPage();
			const auto idx = getRndIdx();
			const auto move = getRndMove();
			const auto value = getRndValue();
			switch (move) {
			case 0:
			{
				auto [[maybe_unused]] v = vm.read(page).at(idx);
				break;
			}
			case 1:
				vm.write(page, idx, value);
				break;
			default:
				break;
			}
		}
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << '\n';
		return -1;
	}
	return 0;
}

void autotest() {
	std::cout << fuzzing() << '\n';
}


void ui() {
	VirtualMemory vm;
	vm.printInfo();

	while (1) {
		std::cout << "1 [page] to read | 2 [page] [idx] [value] to write\n";
		int choice, page_id;
		std::cin >> choice >> page_id;
		switch (choice) {
		case 1:
		{
			const auto& page = vm.read(page_id);
			for (auto v : page)
				std::cout << v << ' ';
			std::cout << '\n';
			vm.printInfo();
			break;
		}
		case 2:
			int value, idx;
			std::cin >> idx >> value; 
			vm.write(page_id, idx, value);
			vm.printInfo();
			break;
		default:
			std::cout << "bad input\n";
		}
	}
}
