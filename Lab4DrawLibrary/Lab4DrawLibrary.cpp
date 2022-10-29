#include "Lab4DrawLibrary.h"

ServerSlots* ServerSlots::getServerSlots() {
	const auto _filemapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof ServerSlots, L"DrawServers");
	const auto already_exists = GetLastError();
	std::cout << "exists = " << (already_exists == ERROR_ALREADY_EXISTS) << '\n';
	if (not _filemapping) {
		std::cout << "mapping error: " << already_exists;
		std::exit(1);
	}
	const auto _shared = MapViewOfFile(_filemapping, FILE_MAP_ALL_ACCESS, 0, 0, sizeof ServerSlots);
	if (not _shared) {
		std::cout << "view error: " << GetLastError() << '\n';
		std::exit(2);
	}
	const auto s_ptr = reinterpret_cast<ServerSlots*>(_shared);

	if (already_exists != ERROR_ALREADY_EXISTS)
		std::construct_at(s_ptr);

	return s_ptr;
}

ServerSlots::Slot& ServerSlots::getServerSlot() {
	const auto slot_iter = std::find_if(slots.begin(), slots.end(), [](Slot& slot) {
		return slot.working.try_acquire();
		}
	);
	assert(slot_iter != slots.end());
	auto& slot = *slot_iter;


	const auto formatted = std::format("SlotSemaphore{}", std::distance(slots.begin(), slot_iter));
	const auto f_str = formatted.c_str();
	strcpy_s(slot.event_name, f_str);

	return slot;
}


ServerSlots::Slot& ServerSlots::getClientSlot() {
	const auto slot_iter = std::find_if(slots.begin(), slots.end(), [](Slot& slot) {
		return slot.acquired.try_acquire();
		}
	);
	assert(slot_iter != slots.end());
	auto& slot = *slot_iter;

	return static_cast<Slot&>(slot);
}
