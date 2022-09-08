#include <iostream>
#include <algorithm>
#include <numeric>
#include <cstddef>
#include <ranges>
#include <vector>
#include <unordered_map>

#include <tl/generator.hpp>
#include <tl/adjacent.hpp>
#include <tl/functional/curry.hpp>
#include <optional>

//#include "Allocator.h"

//#include "BasicAllocator.h"

//import Allocator;
//import Bitmap;
//import LeastSuitable;
//import Blocklist;

#include "NewAllocator.h"
#include "LeastSuitable.h"
#include "Bitmap.h"


#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>


//TEST_CASE("лоль") {
//
//
//}


//
//template<
//	typename Storage,
//	template<typename> typename Strategy
//>
//class BasicAllocator :
//	public Strategy<Storage>
//{
//public:
//	using Strategy_t = Strategy<Storage>;
//
//	BasicAllocator(std::size_t size) : Strategy_t(size) {}
//
//	//auto allocate(std::size_t allocation_size) {
//	//	return allocate(allocation_size);
//	//}
//	//auto deallocate(void* ptr) {
//	//	return deallocate(ptr);
//	//}
//	//auto getInfo() {
//	//	return getInfo();
//	//}
//};




//int main() {
//
//	Allocator<Bitmap, LeastSuitable> a(100);
//
//	const auto ptr = a.allocate(1);
//	a.deallocate(ptr);
//
//}

using allocator = Allocator<std::size_t, Bitmap, LeastSuitable>;


TEST_CASE("Empty in the middle", "[Allocator]") {
	allocator a(100);
	a.allocate(24);
	auto info = a.storage.getInfo();
	std::cout << info.view << '\n';

	auto ptr = a.allocate(49);
	info = a.storage.getInfo();
	std::cout << info.view << '\n';

	a.allocate(24);
	info = a.storage.getInfo();
	std::cout << info.view << '\n';


	a.deallocate(ptr);
	info = a.storage.getInfo();
	std::cout << info.view << '\n';

	REQUIRE(info.used_regions == 2);
	REQUIRE(info.free_regions == 1);
	REQUIRE(info.used_size == 50);
	REQUIRE(info.free_size == 50);
}

TEST_CASE("Overflow", "[Allocator]") {
	allocator a(100);
	a.allocate(99);

	REQUIRE_THROWS_AS(a.allocate(1), std::runtime_error);
	const auto info = a.storage.getInfo();
	std::cout << info.view << '\n';
}

TEST_CASE("Allocations", "[Allocator]") {
	allocator a(100);

	a.allocate(20);
	a.allocate(33);
	a.allocate(44);

	const auto info = a.storage.getInfo();

	std::cout << info.view << '\n';
}


TEST_CASE("Allocate more than exists", "[Allocator]") {
	allocator a(100);

	REQUIRE_THROWS_AS(a.allocate(256), std::runtime_error);
}



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

