#pragma once
#include <vector>
#include <list>
#include <string>






//template<typename Store>
//class FirstSuitable {
//	auto find();
//};



//class MostSuitable {};
//class BinaryPartition {};


//template<typename T>
//concept Allocator = requires(T allocator) {
//	{ allocator.allocate(std::size_t(1)) } -> std::convertible_to<void*>;
//	allocator.deallocate(nullptr);
//};

//template<typename Storage, typename Strategy>


//class Memory {
//public:
//	std::unique_ptr<char> ptr;
//	std::size_t size;
//	Memory(std::size_t size) : ptr(new char(size)), size(size) {}
//};
//
//class Bitmap {
//public:
//	Memory memory;
//	Bitmap(std::size_t size) : bitmap(size, false), memory(size) { }
//	std::vector<bool> bitmap;
//	struct Info {
//		std::size_t used_regions{};
//		std::size_t free_regions{};
//		std::size_t used_size{};
//		std::size_t free_size{};
//		std::string view{};
//	};
//	Info getInfo() {
//		return Info{};
//	}
//};
//
//class BlockList {
//public:
//	Memory memory;
//	BlockList(std::size_t size) : memory(size) { }
//	struct Block {
//		bool used = false;
//		char user = ' ';
//		void* ptr = nullptr;
//		std::size_t size;
//	};
//	std::list<Block> blocklist;
//	struct Info {
//		std::size_t used_regions{};
//		std::size_t free_regions{};
//		std::size_t used_size{};
//		std::size_t free_size{};
//	};
//	Info getInfo() {
//		return Info{};
//	}
//};
//
//
//template<typename Storage>
//class LeastSuitable;
//
//template <>
//class LeastSuitable<Bitmap> : private Bitmap {
//public:
//	using Bitmap::Bitmap;
//	using Bitmap::getInfo;
//	auto allocate(std::size_t allocation_size) {
//		auto asBitmap = static_cast<Bitmap*>(this);
//		auto& bitmap = asBitmap->bitmap;
//
//		using iter_t = std::vector<bool>::iterator;
//		std::optional<std::pair<iter_t, iter_t>> best;
//		std::optional<iter_t> prev;
//		for (
//			auto iter = bitmap.begin();
//			iter != bitmap.end();
//			iter = std::adjacent_find(iter + 1, bitmap.end(), std::not_equal_to{})) {
//
//
//			if (iter == bitmap.begin() and *iter)
//				continue;
//
//			if (prev) {
//				auto begin = std::exchange(prev, std::nullopt).value();
//				auto end = std::next(iter, iter != bitmap.begin());
//				auto dist = std::distance(begin, end);
//				if (std::cmp_less(dist, allocation_size))
//					continue;
//				if (best) {
//					const auto best_dist = std::distance(best->first, best->second);
//					if (dist < best_dist)
//						best = { begin, end };
//				}
//				else
//					best = { begin, end };
//			}
//			else
//				prev = std::next(iter, iter != bitmap.begin());
//		}
//
//
//
//		if (not best) {
//			if (not prev)
//				throw std::runtime_error("not en0ugh memory");
//			else
//				if (std::distance(prev.value(), bitmap.end()) < allocation_size)
//					throw std::runtime_error("not en0ugh memory");
//				else
//					best.emplace(prev.value(), bitmap.end());
//		}
//
//		std::cout << "all0cated " << allocation_size << " at " << std::distance(bitmap.begin(), best->first) << '\n';
//		std::fill_n(best->first, allocation_size, true);
//		const auto owner = std::distance(bitmap.begin(), best->first);
//		//allocations.emplace_back(owner, allocation_size);
//
//		return owner;
//	}
//};
//
//
//template<
//	typename AllocationStorage,
//	template<typename> typename AllocationStrategy
//>
//using BasicAllocator = AllocationStrategy<AllocationStorage>;


//template<typename Derived>
//class Base {
//public:
//	void interface_method() {
//		static_cast<Derived&>(*this).implementation_method();
//	}
//};
//
//class Child : public Base<Child> 
//{
//public:
//	void implementation_method() {
//
//	}
//};
//
//
//template<typename Derived>
//void use(Base<Derived>& obj) {
//
//}
