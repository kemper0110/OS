#pragma once
#include <memory>
#include <list>
#include <vector>
#include <string>
#include <ranges>
#include <algorithm>


struct Memory {
	std::unique_ptr<char[]> ptr;
	std::size_t size;
	Memory(std::size_t size) : ptr(std::make_unique<char[]>(size)), size(size) {}
	void* get(std::size_t id = 0) {
		return ptr.get() + id;
	}
};

