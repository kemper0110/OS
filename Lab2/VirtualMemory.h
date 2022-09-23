#pragma once

#include "Memory.h"
#include "InterruptionStrategy.h"




template <InterruptionStrategy Strategy>
class VirtualMemory {
	Strategy strategy;
	

	Ram ram;
	Hdd hdd;


	//void memoryInterruption(Ram::InfoBlock::PageInfo& requested_page_info) {
	//	while (ram.info.queue.front().R) {
	//		ram.info.queue.front().R = false;
	//		ram.info.queue.shift_forward();
	//	}
	//	auto& oldest_page_info = ram.info.queue.front();
	//	ram.info.queue.pop_front();
	//	std::swap(hdd.pages[requested_page_info.real], ram.pages[oldest_page_info.real]);
	//	oldest_page_info.Type = oldest_page_info.HDD;
	//	oldest_page_info.R = oldest_page_info.M = false;
	//	requested_page_info.Type = requested_page_info.RAM;
	//	std::swap(requested_page_info.real, oldest_page_info.real);
	//	ram.info.queue.push_back(requested_page_info);
	//}
public:
	VirtualMemory() : strategy(ram, hdd) {
		
	}

	Page read(int virtual_address) {
		if (refresh())
			clearRM();

		auto& info = ram.info.table[virtual_address];
		std::cout << "try page #" << virtual_address << '\n';
		if (info.Type == info.HDD) {
			std::cout << "memory miss\n";

			memoryInterruption(info);

			info.R = true; info.M = false;

			return ram.pages[info.real];
		}
		else {
			std::cout << "memory hit\n";
			info.R = 1;
			return ram.pages[info.real];
		}
	}

	void write(int virtual_address, std::size_t idx, int value) {
		if (refresh())
			clearRM();

		auto& info = ram.info.table[virtual_address];
		std::cout << "try page #" << virtual_address << '\n';
		if (info.Type == info.HDD) {
			std::cout << "memory miss\n";

			memoryInterruption(info);

			info.R = 0; info.M = 1;

			ram.pages[info.real][idx] = value;
		}
		else {
			std::cout << "memory hit\n";
			info.M = 1;
			ram.pages[info.real][idx] = value;
		}
	}

	void printInfo() {

	}
};