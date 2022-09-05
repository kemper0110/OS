#pragma once
#include "Memory.h"


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