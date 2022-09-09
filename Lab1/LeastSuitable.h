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

#include "Allocator.h"

#include "Bitmap.h"

#include "RegionView.h"


/*
template<
	typename T,
	template<typename> typename Storage,
	template<typename, typename> typename Strategy
>
struct Allocator
*/

template<
	typename T,
	typename Storage
>
struct LeastSuitable;




template<typename T, std::size_t BLOCKSIZE>
struct LeastSuitable<T, Bitmap<BLOCKSIZE>>{
private:
	static std::size_t ceilSize(std::size_t n) {
		return ceilBytes(sizeof(T) * n + sizeof(std::size_t));
	}
	static std::size_t ceilBytes(std::size_t n) {
		const auto [quot, rem] {
			std::lldiv(n, BLOCKSIZE)
		};
		return quot + (rem == 0 ? 0 : 1);
	}
public:
	auto allocator() {
		return static_cast<
			Allocator<T, Bitmap<BLOCKSIZE>, LeastSuitable>*
		>(this);
	}
	auto allocate(std::size_t size) {
		using iter_t = std::vector<bool>::iterator;

		auto& bitmap = allocator()->storage.bitmap;

		const auto bitmap_size = ceilSize(size);

		std::optional<std::pair<iter_t, iter_t>> best;

		for (const auto [begin, end] : RegionView{ bitmap }) {
			const auto distance = std::distance(begin, end);

			if(std::cmp_less(distance, bitmap_size))	//	!!!
				continue;

			if (not best.has_value()) {
				best.emplace(begin, end);
				continue;
			}
			const auto best_distance = std::distance(best->first, best->second);
			if (distance < best_distance)
				best.emplace(begin, end);
		}

		if (not best)
			throw std::runtime_error("not en0ugh memory");

		const auto bitmap_offset = std::distance(bitmap.begin(), best->first);
		assert(std::cout << "all0cated " << bitmap_size << " at " << bitmap_offset << '\n');
		std::fill_n(best->first, bitmap_size, true);

		auto& memory = allocator()->storage.memory;

		const auto actual_start = reinterpret_cast<std::size_t*>(memory.get(bitmap_offset));
		*actual_start = size * sizeof(T);	// remember block size in bytes
		assert(std::cout << "remembered: " << *actual_start << '\n');
		const auto data_start = actual_start + 1;

		return reinterpret_cast<T*>(data_start);
	}
	auto deallocate(void* data_start) {
		if (data_start == nullptr)
			return;

		auto& memory = allocator()->storage.memory;
		auto& bitmap = allocator()->storage.bitmap;

		const auto actual_start = reinterpret_cast<size_t*>(data_start) - 1;

		const auto data_size = *actual_start;

		const auto bitmap_size = ceilBytes(data_size);

		const auto memory_start = memory.get();	// void*
		const auto bitmap_offset = (reinterpret_cast<char*>(actual_start) - reinterpret_cast<char*>(memory_start)) / BLOCKSIZE;


		std::fill_n(bitmap.begin() + bitmap_offset, bitmap_size, false);


		assert(std::cout << "deall0cated " << bitmap_size << " at " << bitmap_offset << '\n');
	}
};

