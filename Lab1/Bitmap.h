#pragma once
#include "Memory.h"

#include "RegionView.h"

template<std::size_t BLOCKSIZE>
struct Bitmap {
	struct MemoryInfo {
		std::size_t used_blocks{};
		std::size_t free_blocks{};
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
		const auto free_blocks = std::ranges::count(bitmap, false);
		const auto used_blocks = bitmap.size() - free_blocks;

		std::string str;
		std::transform(bitmap.begin(), bitmap.end(), std::back_inserter(str), [](auto v) {return v + '0'; });

		//size_t counter_free = 0, counter_used = 0;
		//bool insideUsedRegion{};
		//for (
		//	auto iter = bitmap.begin();
		//	iter != bitmap.end();
		//	iter = std::adjacent_find(iter + 1, bitmap.end(), std::not_equal_to{})) {
		//	if (iter == bitmap.begin())
		//		insideUsedRegion = *iter;
		//	//(insideUsedRegion ? counter_used : counter_free) += 1;
		//	counter_used += insideUsedRegion;
		//	counter_free += !insideUsedRegion;
		//	insideUsedRegion = !insideUsedRegion;
		//}

		return MemoryInfo{
			.used_blocks = used_blocks,
			.free_blocks = static_cast<std::size_t>(free_blocks),
			.used_size = used_blocks * BLOCKSIZE,
			.free_size = static_cast<std::size_t>(free_blocks) * BLOCKSIZE,
			.view = std::move(str)
		};
	}
	constexpr std::size_t getBlockSize() {
		return BLOCKSIZE;
	}

	std::vector<bool> bitmap;
};
