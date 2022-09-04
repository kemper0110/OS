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

#include "BasicAllocator.h"


//#define CATCH_CONFIG_MAIN
//
//#include <catch2/catch.hpp>


//TEST_CASE("лоль") {
//
//
//}

class Memory {
public:
	std::unique_ptr<char> ptr;
	std::size_t size;
	Memory(std::size_t size) : ptr(new char(size)), size(size) {}
};

class Bitmap {
public:
	Memory memory;
	Bitmap(std::size_t size) : bitmap(size, false), memory(size) { }
	std::vector<bool> bitmap;
	struct Info {
		std::size_t used_regions{};
		std::size_t free_regions{};
		std::size_t used_size{};
		std::size_t free_size{};
		std::string view{};
	};
	Info getInfo() {
		return Info{};
	}
};

class BlockList {
public:
	Memory memory;
	BlockList(std::size_t size) : memory(size) { }
	struct Block {
		bool used = false;
		char user = ' ';
		void* ptr = nullptr;
		std::size_t size;
	};
	std::list<Block> blocklist;
	struct Info {
		std::size_t used_regions{};
		std::size_t free_regions{};
		std::size_t used_size{};
		std::size_t free_size{};
	};
	Info getInfo() {
		return Info{};
	}
};


template<typename Storage>
class LeastSuitable;

template <>
class LeastSuitable<Bitmap> : private Bitmap {
public:
	using Bitmap::Bitmap;
	auto allocate(std::size_t allocation_size) {
		auto asBitmap = static_cast<Bitmap*>(this);
		auto& bitmap = asBitmap->bitmap;

		using iter_t = std::vector<bool>::iterator;
		std::optional<std::pair<iter_t, iter_t>> best;
		std::optional<iter_t> prev;
		for (
			auto iter = bitmap.begin();
			iter != bitmap.end();
			iter = std::adjacent_find(iter + 1, bitmap.end(), std::not_equal_to{})) {


			if (iter == bitmap.begin() and *iter)
				continue;

			if (prev) {
				auto begin = std::exchange(prev, std::nullopt).value();
				auto end = std::next(iter, iter != bitmap.begin());
				auto dist = std::distance(begin, end);
				if (std::cmp_less(dist, allocation_size))
					continue;
				if (best) {
					const auto best_dist = std::distance(best->first, best->second);
					if (dist < best_dist)
						best = { begin, end };
				}
				else
					best = { begin, end };
			}
			else
				prev = std::next(iter, iter != bitmap.begin());
		}



		if (not best) {
			if (not prev)
				throw std::runtime_error("not en0ugh memory");
			else
				if (std::distance(prev.value(), bitmap.end()) < allocation_size)
					throw std::runtime_error("not en0ugh memory");
				else
					best.emplace(prev.value(), bitmap.end());
		}

		std::cout << "all0cated " << allocation_size << " at " << std::distance(bitmap.begin(), best->first) << '\n';
		std::fill_n(best->first, allocation_size, true);
		const auto owner = std::distance(bitmap.begin(), best->first);
		//allocations.emplace_back(owner, allocation_size);

		return owner;
	}
};


template<
	typename Storage,
	template<typename> typename Strategy
>
class BasicAllocator :
	private Strategy<Storage>
{
public:
	using Strategy_t = Strategy<Storage>;

	BasicAllocator(std::size_t size) : Strategy_t(size) {}
	auto allocate(std::size_t allocation_size) {
		return Strategy_t::allocate(allocation_size);
	}
	auto deallocate(void* ptr) {
		return Strategy_t::deallocate(ptr);
	}
	auto getInfo() {
		return Strategy_t::getInfo();
	}
};


int main() {
	BasicAllocator<Bitmap, LeastSuitable> a(100);
	auto ptr = a.allocate(10);
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

