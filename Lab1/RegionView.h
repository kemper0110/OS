#pragma once
#include <ranges>
#include <vector>
#include <algorithm>


struct RegionView : public std::ranges::view_interface<RegionView> {
	using vec_t = std::vector<bool>;

	vec_t& vec;

	RegionView(vec_t& vec) : vec(vec) {}
	struct Sentinel {};

	struct Iterator {
		vec_t& vec;
		vec_t::iterator current;

		Iterator(vec_t& vec) : vec(vec), current(vec.begin()) {
			if (*current)
				current = std::adjacent_find(current + 1, vec.end(), std::not_equal_to{});
		}
		Iterator& operator++() {
			current = std::adjacent_find(current + 1, vec.end(), std::not_equal_to{});
		}
		bool operator==(Sentinel) const {
			return current == vec.end();
		}
		vec_t::iterator operator*()const {
			return std::next(current, current == vec.begin());
		}
	};

	Iterator begin() const { return { vec }; }
	Sentinel end() const { return {}; }
};