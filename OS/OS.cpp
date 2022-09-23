#include <fstream>
#include <iostream>
#include <list>
#include <bitset>
#include <bit>
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


	struct Iterator : std::input_iterator_tag {
		using difference_type = std::ptrdiff_t;
		using value_type = int;
		using pointer = int*;
		using reference = int;
		using iterator_category = std::input_iterator_tag;

		friend Sentinel;
		int value, end;
		Iterator(int begin, int end) : value(begin), end(end) {}
		Iterator& operator++() {
			value++;
			return *this;
		}
		int operator*() { return value; }
		friend bool operator==(const Iterator& it, const Sentinel& s) { return it.value == it.end; }
	};
	struct Sentinel {};

	Iterator begin() { return { m_begin, m_end }; }
	Sentinel end() { return {}; }
};


template <class T>
class my_allocator
{
	char data[50U * sizeof T]{};
	std::vector<bool> bitmap = std::vector<bool>(50, false);

public:
	typedef unsigned int size_type;
	typedef signed int difference_type;
	typedef T value_type;

	using propagate_on_container_move_assignment = std::true_type;

	template <class U>
	struct rebind { typedef my_allocator<U> other; };

	my_allocator() = default;

	template <class U>
	my_allocator(const my_allocator<U>&) {

	}
	template <class U>
	my_allocator<T>& operator=(const my_allocator<U>&) {
		return *this;
	}

	T* allocate(size_type n) {
		std::cout << "from " << this << '\n';
		auto begin = bitmap.begin(), end = bitmap.begin();
		while (1) {
			begin = std::find(end, bitmap.end(), false);
			end = std::find(begin, bitmap.end(), true);
			const auto size = std::distance(begin, end);

			if (size >= n )
				break;
			// || end == bitmap.end()
			if (begin == bitmap.end())
				return nullptr;
		}

		std::fill_n(begin, n, true);
		const auto offset = std::distance(bitmap.begin(), begin);

		const auto ptr = reinterpret_cast<T*>(data) + offset;
		std::cout << ptr << '\n';
		return ptr;
	}
	void deallocate(T* ptr, size_t n) {
		const auto offset = ptr - reinterpret_cast<T*>(data);
		std::fill_n(bitmap.begin() + offset, n, false);
	}

};




int main() {

	//my_allocator<std::pair<int, int*>> alloc;

	//std::cout << alloc.allocate(1) << '\n';
	//std::cout << alloc.allocate(1) << '\n';
	//std::cout << alloc.allocate(1) << '\n';
	//std::cout << alloc.allocate(1) << '\n';


	std::list<int, my_allocator<int>> list;

	list.push_back(1);
	list.push_back(2);
	list.push_back(3);

	for (auto v : list)
		std::cout << v << '\n';

	//for (int v : Range{ 0, 10 }) {
	//	std::cout << v << '\n';
	//}

	//Range r{ 0, 10 };
	//std::ranges::for_each(r.begin(), r.end(), [](auto v) {
	//	std::cout << v << '\n';
	//	});

	//auto beg = r.begin();

	//Range::Sentinel s{};
	//beg == s;
	//s == beg;
	//beg != s;
	//s != beg;



	//constexpr bool v1 = std::sentinel_for<Range::Sentinel, Range::Iterator>;



	//constexpr bool v_1 = std::input_or_output_iterator<Range::Iterator>;
	//constexpr bool v__1 = std::indirectly_readable<Range::Iterator>;
	//constexpr bool v_2 = std::weakly_incrementable<Range::Iterator>;
	//constexpr bool v_3 = std::_Signed_integer_like<std::iter_difference_t<Range::Iterator>>;


}