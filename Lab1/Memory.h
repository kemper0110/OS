#pragma once
#include <memory>
#include <list>
#include <vector>
#include <string>
#include <ranges>
#include <algorithm>


template<typename T>
struct Memory {
	std::unique_ptr<T[]> ptr;
	std::size_t size;
	Memory(std::size_t size) : ptr(std::make_unique<T[]>(size)), size(size) {}
	T* get(std::size_t id = 0) {
		return ptr.get() + id;
	}
};

