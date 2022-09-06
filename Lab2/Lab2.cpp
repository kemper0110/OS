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

//#include <boost/asio.hpp>
//#include <boost/asio/experimental/as_tuple.hpp>
//
//using namespace boost::asio;
//using namespace experimental;
//using namespace ip;
//
//using default_token = as_tuple_t<use_awaitable_t<>>;
//using tcp_socket = default_token::as_default_on_t<tcp::socket>;


constexpr auto PAGESIZE = 4U;

//struct Page {
//	static_assert(PAGESIZE % sizeof(int) == 0);
//	std::array<int, PAGESIZE / sizeof(int)> data;
//	struct Info {
//		bool R : 1 = 0,
//			M : 1 = 0;
//	} info;
//};

constexpr auto PAGECOUNT = 2U;

using Page =
std::array<int, PAGESIZE / sizeof(int)>;

struct Ram {
	struct InfoBlock {
		struct PageInfo : public slist_base_hook<cache_last<true>> {
			enum : char {
				RAM = 0b0,
				HDD = 0b1,
				Referenced = 0b100,		// при обращении выставляется в 1, через нек. время в 0
				Modified = 0b010,		// при записи
			};
			//union {
			//	bool RMT = RAM;
			//	bool
			//		R : 1,
			//		M : 1,
			//		T : 1;
			//} state;
			//char RMT = RAM;
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

Ram ram;
Hdd hdd;
std::size_t ticks;

constexpr auto REFRESH_TICKS = 3;
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

int read(std::size_t virtual_address, std::size_t offset) {
	if (refresh())
		clearRM();

	auto& info = ram.info.table[virtual_address];
	std::cout << "try page #" << virtual_address << '\n';
	if (info.Type == info.HDD) {
		std::cout << "memory miss\n";

		memoryInteruption(info);

		//info.R = 1; info.M = 0;

		return ram.pages[info.real][offset];
	}
	else {
		std::cout << "memory hit\n";
		info.R = 1;
		return ram.pages[info.real][offset];
	}
}

void write(std::size_t virtual_address, std::size_t offset, int value) {
	if (refresh())
		clearRM();

	auto& info = ram.info.table[virtual_address];
	std::cout << "try page #" << virtual_address << '\n';
	if (info.Type == info.HDD) {
		std::cout << "memory miss\n";

		memoryInteruption(info);

		//info.R = 0; info.M = 1;

		ram.pages[info.real][offset] = value;
	}
	else {
		std::cout << "memory hit\n";
		info.M = 1;
		ram.pages[info.real][offset] = value;
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
			(info.R * 10 | info.M) << '\n';
	}
	std::cout << "\tqueue: ";
	for (const auto& info : ram.info.queue)
		std::cout << info.real << ' ';
	std::cout << "\n\n";
}

void init() {
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

int main()
{
	init();
	printInfo();

	//readPage(5);
	//printInfo();
	//readPage(5);


	while (1) {
		std::cout << "1 [page] to read | 2 [page] [value] to write\n";
		int choice, page;
		std::cin >> choice >> page;
		switch (choice) {
		case 1:
			std::cout << read(page, 0);
			printInfo();
			break;
		case 2:
			int value;
			std::cin >> value;
			write(page, 0, value);
			printInfo();
			break;
		}
	}
}
