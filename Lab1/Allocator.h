#pragma once
#include <iostream>
#include <algorithm>
#include <numeric>
#include <cstddef>
#include <ranges>
#include <vector>
#include <unordered_map>

#include <tl/generator.hpp>
#include <tl/adjacent.hpp>
#include <tl/functional/curry.hpp>
#include <optional>


/*

� ��������
������ ����������� ������ ��������� ������ ������������
�������� ������ ������� �� ����� 256 ����.


��� ���������� � ���������/������� �������� ������ ���������
������ �������

�) �������� ������� ��������� �������. � ������ ������ �������
��������� ����� ����������� �������. ���� ������� �����������
��� ��������� �� �������, ���������� ������� ���������������
��������� � �������� ������.
�) ���������� ����� ���������� �������. � �������� ���������
������� ������ ��������� ��������� ����� ��������������
�������. ����� ���������� ������� ����� ���� ����������
������ ������� (������������ ����� ������� �� �����������).

�) �������� ���������� � ���������/������� �������� � ������������ ������
*/
class Allocator {
public:
	std::vector<bool> bitmap;
	struct Allocation {
		std::size_t owner;
		std::size_t size;
		friend bool operator==(const Allocation& lhs, const Allocation& rhs) {
			return lhs.owner == rhs.owner && lhs.size == rhs.size;
		}
	};
	std::vector<Allocation> allocations;

	struct MemoryInfo {
		std::size_t used_regions;
		std::size_t free_regions;
		std::size_t used_size;
		std::size_t free_size;
		std::vector<Allocation> allocations;
		std::string view;
	};

	Allocator(std::size_t size);
	std::size_t allocate(std::size_t allocation_size);
	void deallocate(std::size_t ptr);
	MemoryInfo getInfo() const;
};




//
//tl::generator<const std::pair<std::vector<bool>::iterator, std::vector<bool>::iterator>> get_all_areas() {
//	std::optional<std::vector<bool>::iterator> prev;
//	for (
//		auto iter = bitmap.begin();
//		iter != bitmap.end();
//		iter = std::adjacent_find(iter, bitmap.end(), std::not_equal_to{}))
//		if (iter == bitmap.begin())
//			if (*iter) // skip begin if it's occupied
//				continue;
//			else
//				if (prev)
//					co_yield{ std::exchange(prev, std::nullopt).value(), iter };
//				else
//					prev = iter;
//		else
//			if (prev)
//				co_yield{ std::exchange(prev, std::nullopt).value(), std::next(iter, 1) };
//			else
//				prev = std::next(iter, 1);
//}