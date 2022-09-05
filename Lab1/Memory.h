#pragma once
#include <memory>
#include <list>
#include <vector>
#include <string>
#include <ranges>
#include <algorithm>

struct Memory {
	std::unique_ptr<size_t[]> ptr;
	size_t size;
	Memory(size_t size) : ptr(std::make_unique<size_t[]>(size)), size(size) {}
	size_t* get(size_t id = 0) {
		return ptr.get() + id;
	}
};

