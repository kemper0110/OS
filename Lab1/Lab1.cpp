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

//#include "NewAllocator.h"
#include "LeastSuitable.h"

//#define CATCH_CONFIG_MAIN
//
//#include <catch2/catch.hpp>


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




int main() {

	Allocator<Bitmap, LeastSuitable> a(100);

	a.allocate(1);
	a.deallocate(nullptr);


	//BasicAllocator<Bitmap, LeastSuitable> a(100);
	//auto ptr = a.allocate(10);
}


//TEST_CASE("Empty in the middle", "[Allocator]") {
//	std::cout << "Empty in the middle\n";
//	Allocator a(100);
//	a.allocate(25);
//	auto ptr = a.allocate(50);
//	a.allocate(25);
//
//	a.deallocate(ptr);
//
//	const auto info = a.getInfo();
//	std::cout << info.view << '\n';
//
//	REQUIRE(info.used_regions == 2);
//	REQUIRE(info.free_regions == 1);
//	REQUIRE(info.used_size == 50);
//	REQUIRE(info.free_size == 50);
//	std::cout << '\n';
//}
//
//TEST_CASE("Overflow", "[Allocator]") {
//	std::cout << "Overflow\n";
//	Allocator a(100);
//	a.allocate(100);
//
//	REQUIRE_THROWS_AS(a.allocate(1), std::runtime_error);
//	const auto info = a.getInfo();
//	std::cout << info.view << '\n';
//	std::cout << '\n';
//}
//
//TEST_CASE("Allocations", "[Allocator]") {
//	std::cout << "Allocations\n";
//	Allocator a(100);
//
//	a.allocate(25);
//	
//	a.allocate(30);
//	a.allocate(45);
//
//	const auto info = a.getInfo();
//
//	REQUIRE(
//		info.allocations == decltype(info.allocations) {
//			{ 0, 25 }, { 25, 30 }, { 55, 45 }
//	});
//	std::cout << info.view << '\n';
//	std::cout << '\n';
//
//
//	//std::cout << "owners\tsize\n";
//	//for (const auto& [owner, size] : info.allocations) 
//		//std::cout << owner << '\t' << size << '\n';
//
//}
//
//
//TEST_CASE("Allocate more than exists", "[Allocator]") {
//	std::cout << "Allocate more than exists\n";
//	Allocator a(100);
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

