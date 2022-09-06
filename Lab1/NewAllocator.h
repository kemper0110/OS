#pragma once
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

#include "Memory.h"


template<
	typename T,
	template<typename> typename Storage,
	template<typename, typename> typename Strategy
>
struct Allocator : Strategy<T, Storage<T>> {
	Storage<T> storage;
	Allocator(size_t size) : storage(size) {}
};


//
//template<
//	typename Storage,
//	template<typename> typename Strategy
//>
//struct StrategyBase {
//public:
//	Allocator<Storage, Strategy<Storage>>* allocator() {
//		return static_cast<Allocator<Storage, Strategy<Storage>>*>(this);
//	}
//
//	//Allocator<Storage, LeastSuitable<Storage>>* allocator() {
//	//	return static_cast<Allocator<Storage, LeastSuitable<Storage>>*>(this);
//	//}
//};



