#pragma once
#include <memory>
#include <list>
#include <vector>
#include <string>
#include <ranges>
#include <algorithm>

struct Memory {
	std::unique_ptr<size_t> ptr;
	size_t size;
	Memory(size_t size) : ptr(new size_t(size)), size(size) {}
	size_t* get(size_t id = 0) {
		return ptr.get() + id;
	}
};

struct Blocklist {
	struct Info {
		std::size_t used_regions{};
		std::size_t free_regions{};
		std::size_t used_size{};
		std::size_t free_size{};
	};
	Memory memory;
	Blocklist(size_t size) : memory(size) {}

	struct Block {
		bool used = false;
		char user = ' ';
		void* ptr = nullptr;
		std::size_t size;
	};
	std::list<Block> blocklist;
	Info getInfo() {
		return {};
	}
};

struct Bitmap {
	struct MemoryInfo {
		std::size_t used_regions{};
		std::size_t free_regions{};
		std::size_t used_size{};
		std::size_t free_size{};
		std::string view{};
	};
	Memory memory;
	Bitmap(size_t size) : memory(size) {}
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

			counter_used += insideUsedRegion;
			counter_free += !insideUsedRegion;

			//(insideUsedRegion ? counter_used : counter_free) += 1;
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
	std::vector<bool> bitmap;
};
