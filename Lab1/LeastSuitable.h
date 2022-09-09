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
	static std::size_t sizeToBlocks(std::size_t n) {
		return bytesToBlocks(sizeof(T) * n + sizeof(std::size_t));
	}
	static std::size_t bytesToBlocks(std::size_t n) {
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

		const auto blocks_requested = sizeToBlocks(size);
		//std::cout << "blocks allocated: " << blocks_requested << '\n';

		std::optional<std::pair<iter_t, iter_t>> best;

		for (const auto [begin, end] : RegionView{ bitmap }) {
			const auto distance = std::distance(begin, end);

			if(std::cmp_less(distance, blocks_requested))	//	!!!
				continue;

			if (not best.has_value()) {
				best.emplace(begin, end);
				continue;
			}
			const auto best_distance = std::distance(best->first, best->second);
			if (distance < best_distance)
				best.emplace(begin, end);
		}

		if (not best.has_value())
			throw std::runtime_error("not enough memory");

		std::fill_n(best->first, blocks_requested, true);

		auto& memory = allocator()->storage.memory;

		const auto actual_start = reinterpret_cast<std::size_t*>(memory.get(std::distance(bitmap.begin(), best->first) * BLOCKSIZE));
		*actual_start = blocks_requested;	// remember block size in bytes
		const auto data_start = actual_start + 1;

		/*std::cout << "actual start: " << actual_start << '\n';
		std::cout << "data start: " << data_start << '\n';
		std::cout << "data end: " << data_start + blocks_requested * BLOCKSIZE << '\n';*/

		return reinterpret_cast<T*>(data_start);
	}
	auto deallocate(void* data_start) {
		if (data_start == nullptr)
			return;

		auto& memory = allocator()->storage.memory;
		auto& bitmap = allocator()->storage.bitmap;

		const auto actual_start = reinterpret_cast<size_t*>(data_start) - 1;

		const auto blocks_used = *actual_start;

		const auto memory_start = memory.get();	// void*
		const auto bitmap_offset = (reinterpret_cast<char*>(actual_start) - reinterpret_cast<char*>(memory_start)) / BLOCKSIZE;

		std::fill_n(bitmap.begin() + bitmap_offset, blocks_used, false);

		//std::cout << "deall0cated from: " << actual_start << " blocks: " << blocks_used << '\n';
	}
};

