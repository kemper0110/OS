#include <iostream>
#include <vector>
#include <cstddef>
#include <array>
#include <bitset>

//#include <boost/asio.hpp>
//#include <boost/asio/experimental/as_tuple.hpp>
//
//using namespace boost::asio;
//using namespace experimental;
//using namespace ip;
//
//using default_token = as_tuple_t<use_awaitable_t<>>;
//using tcp_socket = default_token::as_default_on_t<tcp::socket>;


struct Page {
	std::array<int, 32U / sizeof(int)> data;
	struct Info {
		bool R : 1 = 0,
			M : 1 = 0;
	} info;
};

struct Ram {
	struct InfoBlock {
		std::bitset<32 * 2> RM;
	} info;
	struct Page {
		std::array<int, 32U / sizeof(int)> data;
	};
	std::array<Page, 32> pages;

};


std::array<Page, 8> ram;
std::array<Page, 8> hdd;

// FIFO

// queue in ram

class MinAllocation {

};

template <typename AllocationStrategy = MinAllocation>
class Allocator;

template <typename AllocationStrategy>
class Allocator {

};


int main()
{
	sizeof Page::Info;

	Allocator a;
}
