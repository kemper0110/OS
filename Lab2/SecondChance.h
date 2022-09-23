#pragma once
#include <sstream>
#include <queue>
#include <string>
#include "Memory.h"

// построение кодового дерева
// кодирование/декодирование

struct SecondChance {
	struct IterableQueue : std::queue<Ram::InfoBlock::PageInfo*> {
		friend std::ostream& operator<<(std::ostream& os, const IterableQueue& queue) {
			for (const auto& v : queue.c)
				os << v->real << ' ';
			return os;
		}
	} queue;
	Ram& ram;
	Hdd& hdd;
	
	constexpr static int REFRESH_TICKS = 4;

	std::size_t ticks = 0;

	bool refresh() {
		return (++ticks %= (REFRESH_TICKS + 1)) == REFRESH_TICKS;
	}
	void clearRM() {
		for (auto& info : ram.info.table)
			if (info.Type == info.RAM)
				info.R = info.M = false;
	}

	SecondChance(Ram& ram, Hdd& hdd) : ram(ram), hdd(hdd) {
		for (int i = 0; i < PAGECOUNT; ++i) {
			ram.info.table[i].Type = Ram::InfoBlock::PageInfo::RAM;
			ram.info.table[i].real = i;
			ram.info.table[PAGECOUNT + i].Type = Ram::InfoBlock::PageInfo::HDD;
			ram.info.table[PAGECOUNT + i].real = i;
		}
		for (auto& pageinfo : ram.info.table) 
			if (pageinfo.Type == pageinfo.RAM)
				queue.push(&pageinfo);
	}

	std::string info() const {
		std::ostringstream oss;

		oss << "\n\ttable\n";
		oss << std::setw(4) << "id" << std::setw(7) << "where" << std::setw(6) << "real" << std::setw(4) << "val" << std::setw(4) << "RM" << '\n';

		for (int i = 0; i < ram.info.table.size(); ++i) {
			const auto& info = ram.info.table[i];
			oss << std::setw(4) << i << std::setw(7) << (info.Type ? " HDD " : " RAM ") << std::setw(6) <<
				info.real << std::setw(4) <<
				(info.Type ? hdd.pages[info.real][0] : ram.pages[info.real][0])
				<< std::setw(4) <<
				(info.R * 10 | (int)info.M) << '\n';
		}
		oss << "\tqueue: ";
		oss << queue;
		oss << "\n\n";
	}

	void interrupt(int requested) {
		auto* requested_page_info = &ram.info.table[requested];
		while (queue.front()->R) {
			auto info = queue.front();
			//queue.front()->R = false;
			info->R = false;
			//queue.shift_forward();
			queue.pop();
			queue.push(info);
		}
		auto oldest_page_info = queue.front();
		queue.pop();
		std::swap(hdd.pages[requested_page_info->real], ram.pages[oldest_page_info->real]);

		oldest_page_info->Type = oldest_page_info->HDD;
		oldest_page_info->R = oldest_page_info->M = false;
		requested_page_info->Type = requested_page_info->RAM;

		std::swap(requested_page_info->real, oldest_page_info->real);
		queue.push(requested_page_info);
	}
};