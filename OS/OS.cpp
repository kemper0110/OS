#include <fstream>
#include <list>
#include <bitset>
#include <algorithm>
#include <numeric>
#include <vector>

//  : public std::allocator<T>
template<size_t STACKSIZE, typename T>
class allocator {
	T data[STACKSIZE];
	std::vector<bool> bitmap;
public:
	using value_type = T;
	using size_type = size_t;
	using pointer = T*;
	using const_pointer = const T*;

	template<typename _Tp1>
	struct rebind
	{
		using other = allocator<STACKSIZE, _Tp1>;
	};

	allocator() noexcept : bitmap(STACKSIZE) {}
	~allocator() noexcept {}

	allocator(const allocator& a) noexcept : allocator() {
		//bitmap = a.bitmap;
		//std::copy(a.data, a.data + STACKSIZE, data);
	}

	template<size_t stacksize, class U>
	allocator(const allocator& a) noexcept : allocator() {
		//static_assert(sizeof(U) == sizeof(T));
		//bitmap = a.bitmap;
		//std::copy(a.data, a.data + STACKSIZE, data);
	}

	pointer allocate(size_type n) {
		using iter_t = std::vector<bool>::iterator;

		iter_t begin, end;

		end = begin = bitmap.begin();

		while (end - begin < n && begin != bitmap.end()) {
			begin = std::find(begin, bitmap.end(), false);
			end = std::find(begin, bitmap.end(), true);
		}
		if (begin == bitmap.end()) 
			throw "no memory";

		const auto offset = begin - bitmap.begin();
		return data + offset;
	}
	void deallocate(pointer ptr, size_type n) noexcept {
		//std::destroy_n(ptr, n);
		const auto offset = ptr - data;
		std::fill_n(bitmap.begin() + offset, n, false);
	}
};

//template<typename T>
//using allocatorT = allocator<1024, T>;

int main() {
	
	allocator<1024, int> all;
	std::vector<int, allocator<1024, int>> vec;

	//std::list<int, allocator<1024, int>> list(all);
	//list.push_back(1);



}