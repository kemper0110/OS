//#include "Allocator.h"

//
//Allocator::Allocator(std::size_t size) : bitmap(size, false) {}
//
//
//
//std::size_t Allocator::allocate(std::size_t allocation_size) {
//	using iter_t = std::vector<bool>::iterator;
//
//	//const auto rng = RegionView{ bitmap };
//	//for (
//	//	auto iter = rng.begin();
//	//	iter != rng.end(); ) {
//	//	const auto begin = *iter; ++iter;
//	//	const auto end = *iter; ++iter;
//	//}
//
//	std::optional<std::pair<iter_t, iter_t>> best;
//	std::optional<iter_t> prev;
//	for (
//		auto iter = bitmap.begin();
//		iter != bitmap.end();
//		iter = std::adjacent_find(iter + 1, bitmap.end(), std::not_equal_to{})) {
//
//
//		if (iter == bitmap.begin() and *iter)
//			continue;
//
//		if (prev) {
//			auto begin = std::exchange(prev, std::nullopt).value();
//			auto end = std::next(iter, iter != bitmap.begin());
//			auto dist = std::distance(begin, end);
//			if (std::cmp_less(dist, allocation_size))
//				continue;
//			if (best) {
//				const auto best_dist = std::distance(best->first, best->second);
//				if (dist < best_dist)
//					best = { begin, end };
//			}
//			else
//				best = { begin, end };
//		}
//		else
//			prev = std::next(iter, iter != bitmap.begin());
//	}
//
//
//
//	if (not best) {
//		if (not prev)
//			throw std::runtime_error("not en0ugh memory");
//		else 
//			if(std::distance(prev.value(), bitmap.end()) < allocation_size)
//				throw std::runtime_error("not en0ugh memory");
//			else
//				best.emplace(prev.value(), bitmap.end());
//	}
//
//	std::cout << "all0cated " << allocation_size << " at " << std::distance(bitmap.begin(), best->first) << '\n';
//	std::fill_n(best->first, allocation_size, true);
//	const auto owner = std::distance(bitmap.begin(), best->first);
//	allocations.emplace_back(owner, allocation_size);
//
//	return owner;
//}
//
//
//
//void Allocator::deallocate(std::size_t ptr) {
//	const auto allocation = std::ranges::find(allocations, ptr, &Allocation::owner);
//	if (allocation == allocations.end())
//		throw std::runtime_error("ptr not found");
//	std::fill_n(bitmap.begin() + allocation->owner, allocation->size, false);
//	std::cout << "deall0cated " << allocation->size << " at " << allocation->owner << '\n';
//}
//
//
//
//
//Allocator::MemoryInfo Allocator::getInfo() const {
//	const auto free = std::ranges::count(bitmap, true);
//	const auto allocated = bitmap.size() - free;
//
//
//	std::string str;
//	std::transform(bitmap.begin(), bitmap.end(), std::back_inserter(str), [](auto v) {return v + '0'; });
//
//	size_t counter_free = 0, counter_used = 0;
//	bool insideUsedRegion{};
//	for (
//		auto iter = bitmap.begin();
//		iter != bitmap.end();
//		iter = std::adjacent_find(iter + 1, bitmap.end(), std::not_equal_to{})) {
//
//		if (iter == bitmap.begin())
//			insideUsedRegion = *iter;
//
//		counter_used += insideUsedRegion;
//		counter_free += !insideUsedRegion;
//
//		//(insideUsedRegion ? counter_used : counter_free) += 1;
//		insideUsedRegion = !insideUsedRegion;
//	}
//
//	return MemoryInfo{
//		.used_regions = counter_used,
//		.free_regions = counter_free,
//		.used_size = allocated,
//		.free_size = static_cast<std::size_t>(free),
//		.allocations = allocations,
//		.view = std::move(str)
//	};
//}