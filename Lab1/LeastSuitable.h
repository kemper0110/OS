#pragma once
#include <cassert>

#include <memory>
#include <list>
#include <vector>
#include <string>

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <functional>

#include "NewAllocator.h"

template<typename Storage>
struct LeastSuitable {
	auto allocate(std::size_t size);
	auto deallocate(size_t* ptr);

	Allocator<Storage, LeastSuitable<Storage>>* allocator() {
		return static_cast<Allocator<Storage, LeastSuitable<Storage>>*>(this);
	}
};


template<>
auto LeastSuitable<Bitmap>::allocate(std::size_t allocation_size) {
	using iter_t = std::vector<bool>::iterator;

	auto bitmap = allocator()->storage.bitmap;
	//const auto rng = RegionView{ bitmap };
	//for (
	//	auto iter = rng.begin();
	//	iter != rng.end(); ) {
	//	const auto begin = *iter; ++iter;
	//	const auto end = *iter; ++iter;
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
			if (std::cmp_less(dist, allocation_size + 1))	// + space for allocation_size
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
			if (std::distance(prev.value(), bitmap.end()) < allocation_size + 1)
				throw std::runtime_error("not en0ugh memory");
			else
				best.emplace(prev.value(), bitmap.end());
	}

	std::cout << "all0cated " << allocation_size << " at " << std::distance(bitmap.begin(), best->first) << '\n';
	std::fill_n(best->first, allocation_size, true);
	const auto bitmap_idx = std::distance(bitmap.begin(), best->first);

	auto& memory = allocator()->storage.memory;

	auto actual_start = memory.get(bitmap_idx);

	*actual_start = allocation_size;
	const auto data_start = actual_start + 1;



	return actual_start;
}

template<>
auto LeastSuitable<Bitmap>::deallocate(size_t* ptr) {
	auto & memory = allocator()->storage.memory;
	auto& bitmap = allocator()->storage.bitmap;

	const auto data_start = ptr;
	const auto data_size = *(ptr - 1);

	const auto actual_start = data_start - 1;
	const auto actual_size = data_size + 1;

	const auto bitmap_start = (ptr - actual_start) / sizeof(std::size_t);
	const auto bitmap_size = actual_size / sizeof(std::size_t);
	//assert(false);
	std::fill_n(bitmap.begin() + bitmap_start, bitmap_size, false);


	std::cout << "deall0cated " << bitmap_size << " at " << bitmap_start << '\n';
}