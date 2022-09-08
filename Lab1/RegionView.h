#pragma once
#include <ranges>
#include <vector>
#include <algorithm>


struct RegionView : public std::ranges::view_interface<RegionView> {
	using vec_t = std::vector<bool>;

	vec_t& bitmap;

	RegionView(vec_t& vec) : bitmap(vec) {}
	struct Sentinel {};

	struct Iterator {
		vec_t& bitmap;
		using iter_t = vec_t::iterator;
		iter_t begin, end;

		void next() {
			if (begin == bitmap.end() || end == bitmap.end()) {
				begin = end = bitmap.end();
				return;
			}
			begin = std::find(begin, bitmap.end(), false);
			end = std::find(begin, bitmap.end(), true);
		}

		Iterator(vec_t& vec) : bitmap(vec), begin(vec.begin()), end(vec.begin()) {
			next();
		}
		Iterator& operator++() {
			next();
			return *this;
		}
		bool operator==(Sentinel) const {
			return begin == bitmap.end();
		}
		std::pair<iter_t, iter_t> operator*() const {
			return { begin, end };
		}
	};

	Iterator begin() const { return { bitmap }; }
	Sentinel end() const { return {}; }
};