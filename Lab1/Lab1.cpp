#include <iostream>
#include <algorithm>
#include <numeric>
#include <cstddef>
#include <ranges>
#include <vector>
#include <unordered_map>
#include <optional>

#include "Allocator.h"
#include "LeastSuitable.h"
#include "Bitmap.h"


//#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

struct X {
	int a, b, c;
};


int main() {
	using allocator = Allocator<std::size_t, Bitmap<8>, LeastSuitable>;
	
	allocator a(256);	// bytes

	auto info = [&a] {
		auto info = a.storage.getInfo();
		std::cout << info.view << '\n';
		std::cout << "used: " << info.used_size << '\n';
	};
	std::vector<std::pair<size_t, size_t>> allocations;
	while (1) {
		std::cout << "\n\n1 [size] allocate | 2 [ptr] deallocate | 3 info\n";

		std::size_t choice, param;
		std::cin >> choice >> param;
		switch (choice) {
		case 1:
		{
			try {
				auto ptr = a.allocate(param);
				auto num = (std::size_t)ptr;
				std::cout << "ptr: " << num << '\n';
				info();
			}
			catch (const std::runtime_error& ex) {
				std::cout << ex.what() << '\n';
				info();
			}
			break;
		}
		case 2:
		{
			auto ptr = (void*)param;
			//std::cout << "input ptr: " << ptr << '\n';
			a.deallocate(ptr);
			info();
			break;
		}
		case 3:
		{
			auto info = a.storage.getInfo();
			std::cout << info.view << '\n';
			break;
		}
		default: 
			std::cin.ignore(32768);
			break;
		}
	}
}

//template struct LeastSuitable<std::size_t, Bitmap<8>>;
//template struct Allocator<std::size_t, Bitmap<8>, LeastSuitable>;

// extern template LeastSuitable<std::size_t, Bitmap<8>>;
// extern template Allocator<std::size_t, Bitmap<8>, LeastSuitable>;



//TEST_CASE("Empty in the middle", "[Allocator]") {
//	allocator a(sizeof(std::size_t) * 100);
//	a.allocate(24);
//	auto info = a.storage.getInfo();
//	std::cout << info.view << '\n';
//
//	auto ptr = a.allocate(49);
//	info = a.storage.getInfo();
//	std::cout << info.view << '\n';
//
//	a.allocate(24);
//	info = a.storage.getInfo();
//	std::cout << info.view << '\n';
//
//	a.deallocate(ptr);
//	info = a.storage.getInfo();
//	std::cout << info.view << '\n';
//
//	std::cout << info.used_regions << ' ';
//	std::cout << info.free_regions << ' ';
//	std::cout << info.used_size << ' ';
//	std::cout << info.free_size << '\n';
//}
//
//TEST_CASE("Overflow", "[Allocator]") {
//	allocator a(sizeof(std::size_t) * 100);
//	a.allocate(99);
//
//	REQUIRE_THROWS_AS(a.allocate(1), std::runtime_error);
//	const auto info = a.storage.getInfo();
//	std::cout << info.view << '\n';
//}
//
//TEST_CASE("Allocations", "[Allocator]") {
//	allocator a(sizeof(std::size_t) * 100);
//
//	a.allocate(20);
//	a.allocate(33);
//	a.allocate(44);
//
//	const auto info = a.storage.getInfo();
//
//	std::cout << info.view << '\n';
//}
//
//
//TEST_CASE("Allocate more than exists", "[Allocator]") {
//	allocator a(sizeof(std::size_t) * 100);
//
//	REQUIRE_THROWS_AS(a.allocate(256), std::runtime_error);
//}



//int main()
//{
//	Allocator a;
//
//	try {
//		a.allocate(20);
//		auto p1 = a.allocate(20);
//		auto p2 = a.allocate(20);
//		a.allocate(20);
//		a.allocate(20);
//		a.deallocate(p1, 20);
//		a.deallocate(p2, 20);
//
//		a.getInfo();
//	}
//	catch (std::runtime_error& err) {
//		std::cout << err.what() << '\n';
//	}
//	//std::from_chars
//
//}

