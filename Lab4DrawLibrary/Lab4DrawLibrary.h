#pragma once

#include <array>
#include <SFML/Graphics.hpp>
#include <string>
#include <semaphore>
#include <iostream>
#include <format>


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cassert>


constexpr auto MAX_SERVER_COUNT = 50UL;
constexpr auto WINDOW_WIDTH = 640UL;
constexpr auto WINDOW_HEIGHT = 480UL;



using Rect = std::array<sf::Vertex, 4>;

struct ServerSlots {
	struct Slot {
		char event_name[50];
		Rect rect;
		std::binary_semaphore working = std::binary_semaphore{ 1 };
		std::binary_semaphore acquired = std::binary_semaphore{ 1 };
	};

	std::array<Slot, MAX_SERVER_COUNT> slots{};

	ServerSlots() = default;
public:
	static ServerSlots* getServerSlots();
	Slot& getServerSlot();
	Slot& getClientSlot();
};