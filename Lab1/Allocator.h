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
	typename Storage,
	template<typename, typename> typename Strategy
>
struct Allocator : Strategy<T, Storage> {
	Storage storage;
	Allocator(std::size_t size) : storage(size) {}
};



//
//template<
//	typename T,
//	typename Storage,
//	template<typename, typename> typename Strategy
//>
//struct StrategyBase {
//	Allocator<T, Storage, Strategy>* allocator2() {
//		return static_cast<Allocator<T, Storage, Strategy>*>(this);
//	}
//};
//  : StrategyBase<T, Bitmap<BLOCKSIZE>, LeastSuitable> 