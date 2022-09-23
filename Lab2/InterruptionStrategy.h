#pragma once
#include <string>
#include "Memory.h"


template<class T>
concept InterruptionStrategy = requires(T & strategy, int idx, int value, int virtual_address) {
	{ strategy.info() } -> std::convertible_to<std::string>;
	{ strategy.interrupt(idx) } -> std::convertible_to<int>;
	{ strategy.write(virtual_address, idx, value) };
	{ strategy.read(virtual_address) } -> std::convertible_to<Ram::InfoBlock>;
};
