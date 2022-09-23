#pragma once
#include <array>


constexpr auto PAGESIZE = 4U;

constexpr auto PAGECOUNT = 4U;

using Page =
std::array<int, PAGESIZE>;


struct Ram {
	struct InfoBlock {
		struct PageInfo {
			enum : bool {
				RAM = false,
				HDD = true,
			};
			bool
				R : 1 = false,
				M : 1 = false,
				Type : 1 = RAM;
			std::size_t real{};
		};
		std::array<PageInfo, 2 * PAGECOUNT> table;
	} 
	info;
	std::array<Page, PAGECOUNT> pages{};
};


struct Hdd {
	std::array<Page, PAGECOUNT> pages{};
};
