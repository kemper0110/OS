#pragma once
#include "Memory.h"

#include "RegionView.h"

template<std::size_t BLOCKSIZE>
struct Bitmap {
	struct MemoryInfo {
		std::size_t used_regions{};
		std::size_t free_regions{};
		std::size_t used_size{};
		std::size_t free_size{};
		std::string view{};
	};
	static std::size_t calcSize(std::size_t bytes) {
		return bytes / BLOCKSIZE;
	}
	Memory memory;
	Bitmap(std::size_t size) : memory(size), bitmap(calcSize(size), false) {}
	MemoryInfo getInfo() {
		const auto free = std::ranges::count(bitmap, true);
		const auto allocated = bitmap.size() - free;

		std::string str;
		std::transform(bitmap.begin(), bitmap.end(), std::back_inserter(str), [](auto v) {return v + '0'; });

		size_t counter_free = 0, counter_used = 0;
		bool insideUsedRegion{};
		for (
			auto iter = bitmap.begin();
			iter != bitmap.end();
			iter = std::adjacent_find(iter + 1, bitmap.end(), std::not_equal_to{})) {

			if (iter == bitmap.begin())
				insideUsedRegion = *iter;

			//(insideUsedRegion ? counter_used : counter_free) += 1;
			counter_used += insideUsedRegion;
			counter_free += !insideUsedRegion;

			insideUsedRegion = !insideUsedRegion;
		}


		return MemoryInfo{
			.used_regions = counter_used,
			.free_regions = counter_free,
			.used_size = allocated,
			.free_size = static_cast<std::size_t>(free),
			.view = std::move(str)
		};
	}
	constexpr std::size_t getBlockSize() {
		return BLOCKSIZE;
	}

	std::vector<bool> bitmap;
};
