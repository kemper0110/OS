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
auto LeastSuitable<Bitmap>::allocate(std::size_t n) {
	using iter_t = std::vector<bool>::iterator;

	auto& bitmap = allocator()->storage.bitmap;
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
			if (std::cmp_less(dist, n + 1))	// + space for allocation_size
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
			if (std::distance(prev.value(), bitmap.end()) < n + 1)
				throw std::runtime_error("not en0ugh memory");
			else
				best.emplace(prev.value(), bitmap.end());
	}

	std::cout << "all0cated " << n + 1 << " at " << std::distance(bitmap.begin(), best->first) << '\n';
	std::fill_n(best->first, n + 1, true);
	const auto bitmap_start = std::distance(bitmap.begin(), best->first);

	auto& memory = allocator()->storage.memory;

	const auto actual_start = memory.get(bitmap_start);
	*actual_start = n;
	const auto data_start = actual_start + 1;

	return data_start;
}

template<>
auto LeastSuitable<Bitmap>::deallocate(size_t* data_start) {
	if (data_start == nullptr) 
		return;

	auto& memory = allocator()->storage.memory;
	auto& bitmap = allocator()->storage.bitmap;

	const auto actual_start = data_start - 1;

	const auto data_size = *actual_start;
	const auto actual_size = data_size + 1;

	const auto memory_start = memory.get();
	const auto bitmap_offset = actual_start - memory_start;

	std::fill_n(bitmap.begin() + bitmap_offset, actual_size, false);


	std::cout << "deall0cated " << actual_size << " at " << bitmap_offset << '\n';
}