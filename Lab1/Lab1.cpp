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


#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

/*

В качестве
модели оперативной памяти программа должна использовать
байтовый массив размера не менее 256 байт.


вся информация о свободных/занятых участках должна храниться
внутри массива

а) Выделить участок заданного размера. В случае успеха вывести
начальный адрес выделенного участка. Если участка подходящего
для выделения не найдено, необходимо вывести диагностическое
сообщение о нехватке памяти.
б) Освободить ранее выделенный участок. В качестве параметра
функция должна принимать начальный адрес освобождаемого
участка. Ранее выделенный участок может быть освобожден
только целиком (освобождение части участка не допускается).

в) Получить информацию о свободных/занятых участках в «оперативной памяти»


*/

//
struct RegionView : public std::ranges::view_interface<RegionView> {
	using vec_t = std::vector<bool>;

	vec_t& vec;

	RegionView(vec_t& vec) : vec(vec) {}
	struct Sentinel {};

	struct Iterator {
		vec_t& vec;
		vec_t::iterator current;

		Iterator(vec_t& vec) : vec(vec), current(vec.begin()) {
			if (*current)
				current = std::adjacent_find(current + 1, vec.end(), std::not_equal_to{});
		}
		Iterator& operator++() {
			current = std::adjacent_find(current + 1, vec.end(), std::not_equal_to{});
		}
		bool operator==(Sentinel) const {
			return current == vec.end();
		}
		vec_t::iterator operator*()const {
			return std::next(current, current == vec.begin());
		}
	};

	Iterator begin() const { return { vec }; }
	Sentinel end() const { return {}; }
};

//
//struct Pairwise {
//
//	Pairwise(Range& rng) : rng(rng) {}
//
//
//	Range& rng;
//	struct Sentinel {};
//
//	struct Iterator {
//		Iterator(Range& rng) : rng(rng), current(rng.begin()) {}
//		Range& rng;
//
//		Range::Iterator current;
//
//		Iterator& operator++() {
//
//		}
//		bool operator==(Sentinel) const {
//			return current == rng.end();
//		}
//		std::pair<Range::Iterator, Range::Iterator> operator*()const {
//			auto cur_begin = current;
//			auto cur_end = cur_begin;
//			cur_end++;
//		}
//	};
//
//
//	Iterator begin() {
//		return { rng.begin() };
//	}
//	Sentinel end() {
//		return {};
//	}
//};






class Allocator {

	tl::generator<const std::pair<std::vector<bool>::iterator, std::vector<bool>::iterator>> get_all_areas() {
		//std::optional<std::vector<bool>::iterator> prev;
		//for (
		//	auto iter = bitmap.begin();
		//	iter != bitmap.end();
		//	iter = std::adjacent_find(iter, bitmap.end(), std::not_equal_to{}))
		//	if (iter == bitmap.begin())
		//		if (*iter) // skip begin if it's occupied
		//			continue;
		//		else
		//			if (prev)
		//				co_yield{ std::exchange(prev, std::nullopt).value(), iter };
		//			else
		//				prev = iter;
		//	else
		//		if (prev)
		//			co_yield{ std::exchange(prev, std::nullopt).value(), std::next(iter, 1) };
		//		else
		//			prev = std::next(iter, 1);
	}


public:
	std::vector<bool> bitmap;
	struct Allocation {
		std::size_t owner;
		std::size_t size;
	};
	std::vector<Allocation> allocations;

	Allocator(std::size_t size) : bitmap(size, false) {}

	std::size_t allocate(std::size_t allocation_size) {
		using iter_t = std::vector<bool>::iterator;


		//const auto rng = RegionView{ bitmap };
		//for (
		//	auto iter = rng.begin();
		//	iter != rng.end(); ) {
		//	auto begin = *iter;

		//}


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
			auto x = bitmap.begin();
			if (not prev)
				throw std::runtime_error("not en0ugh memory");
			else
				best.emplace(prev.value(), bitmap.end());
		}

		std::cout << "all0cated " << allocation_size << " at " << std::distance(bitmap.begin(), best->first) << '\n';
		std::fill_n(best->first, allocation_size, true);
		const auto owner = std::distance(bitmap.begin(), best->first);
		allocations.emplace_back(owner, allocation_size);

		return owner;
	}

	void deallocate(std::size_t ptr) {
		const auto allocation = std::ranges::find(allocations, ptr, &Allocation::size);
		if (allocation == allocations.end())
			throw std::runtime_error("ptr not found");
		std::fill_n(bitmap.begin() + allocation->owner, allocation->size, false);
		std::cout << "deall0cated " << allocation->size << " at " << allocation->owner << '\n';
	}
	auto getInfo() const {
		const auto free = std::ranges::count(bitmap, true);
		const auto allocated = bitmap.size() - free;


		std::string str;
		std::transform(bitmap.begin(), bitmap.end(), std::back_inserter(str), [](auto v) {return v + '0'; });

		size_t counter_free = 0, counter_used = 0;
		bool freeRegionEnd = false;
		for (
			auto iter = bitmap.begin();
			iter != bitmap.end();
			iter = std::adjacent_find(iter + 1, bitmap.end(), std::not_equal_to{})) {

			if (iter == bitmap.begin())
				freeRegionEnd = *iter;
			if (freeRegionEnd)
				counter_used++;
			else
				counter_free++;
			freeRegionEnd = !freeRegionEnd;
		}

		return MemoryInfo{
			.used_regions = counter_used,
			.free_regions = counter_free,
			.used_size = allocated,
			.free_size = static_cast<std::size_t>(free),
			.allocations = allocations,
			.view = std::move(str)
		};
	}

	struct MemoryInfo {
		std::size_t used_regions;
		std::size_t free_regions;
		std::size_t used_size;
		std::size_t free_size;
		std::vector<Allocation> allocations;
		std::string view;
	};
};


TEST_CASE("allocated/free counters") {
	Allocator a(100);
	a.allocate(25);
	auto ptr = a.allocate(50);
	a.allocate(25);

	a.deallocate(ptr);

	const auto info = a.getInfo();
	std::cout << info.view << '\n';

	REQUIRE(info.used_regions == 2);
	REQUIRE(info.free_regions == 1);
	REQUIRE(info.used_size == 50);
	REQUIRE(info.free_size == 50);
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

