#include <fstream>
#include <iostream>
#include <list>
#include <bitset>
#include <algorithm>
#include <numeric>
#include <vector>
#include <ranges>

//  : public std::allocator<T>
//template<size_t STACKSIZE, typename T>
//class allocator {
//	T data[STACKSIZE];
//	std::vector<bool> bitmap;
//public:
//	using value_type = T;
//	using size_type = size_t;
//	using pointer = T*;
//	using const_pointer = const T*;
//
//	template<typename _Tp1>
//	struct rebind
//	{
//		using other = allocator<STACKSIZE, _Tp1>;
//	};
//
//	allocator() noexcept : bitmap(STACKSIZE) {}
//	~allocator() noexcept {}
//
//	allocator(const allocator& a) noexcept : allocator() {
//		//bitmap = a.bitmap;
//		//std::copy(a.data, a.data + STACKSIZE, data);
//	}
//
//	template<size_t stacksize, class U>
//	allocator(const allocator& a) noexcept : allocator() {
//		//static_assert(sizeof(U) == sizeof(T));
//		//bitmap = a.bitmap;
//		//std::copy(a.data, a.data + STACKSIZE, data);
//	}
//
//	pointer allocate(size_type n) {
//		using iter_t = std::vector<bool>::iterator;
//
//		iter_t begin, end;
//
//		end = begin = bitmap.begin();
//
//		while (end - begin < n && begin != bitmap.end()) {
//			begin = std::find(begin, bitmap.end(), false);
//			end = std::find(begin, bitmap.end(), true);
//		}
//		if (begin == bitmap.end()) 
//			throw "no memory";
//
//		const auto offset = begin - bitmap.begin();
//		return data + offset;
//	}
//	void deallocate(pointer ptr, size_type n) noexcept {
//		//std::destroy_n(ptr, n);
//		const auto offset = ptr - data;
//		std::fill_n(bitmap.begin() + offset, n, false);
//	}
//};

//template<typename T>
//using allocatorT = allocator<1024, T>;


struct Range {
	int m_begin, m_end;
	Range(int begin, int end) : m_begin(begin), m_end(end) {}

	struct Sentinel;
	struct Iterator;


	struct Iterator {
		friend Sentinel;
		int value, end;
		Iterator(int begin, int end) : value(begin), end(end) {}
		Iterator& operator++() {
			value++;
			return *this;
		}
		int operator*() { return value; }
		bool operator==(Sentinel s) { return value == end; }
		bool operator!=(Sentinel s) { return value != end; }
	};
	struct Sentinel{
		bool operator==(const Iterator& it) {
			return it.value == it.end;
		}
		bool operator!=(const Iterator& it) {
			return it.value != it.end;
		}
	};

	Iterator begin() { return { m_begin, m_end }; }
	Sentinel end() { return {}; }
};


template <class _Ty1, class _Ty2>
concept equal_nequal =
	//requires(const std::remove_reference_t<_Ty1>&__x, const std::remove_reference_t<_Ty2>&__y) {
	requires(_Ty1 __x, _Ty2 __y) {
		{ __x == __y } -> std::_Boolean_testable;
		{ __x != __y } -> std::_Boolean_testable;
};

int main() {

	for (int v : Range{ 0, 10 }) {
		std::cout << v << '\n';
	}

	Range r{ 0, 10 };
	//std::ranges::for_each(r.begin(), r.end(), [](auto v) {
	//	std::cout << v << '\n';
	//	});

	auto beg = r.begin();

	Range::Sentinel s{};
	beg == s;
	s == beg;
	beg != s;
	s != beg;

	constexpr bool v1 = std::sentinel_for<Range::Sentinel, Range::Iterator>;
	constexpr bool v2 = std::semiregular<Range::Sentinel>;
	constexpr bool v3 = std::_Weakly_equality_comparable_with<Range::Sentinel, Range::Iterator>;
	constexpr bool v4 = std::_Half_equality_comparable<Range::Sentinel, Range::Iterator>;

	//semiregular<_Se>
	//	&& input_or_output_iterator<_It>
	//	&& _Weakly_equality_comparable_with<_Se, _It>;

}