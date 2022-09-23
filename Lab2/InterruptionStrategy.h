#pragma once
#include <string>


template<class T>
concept InterruptionStrategy = requires(const T strategy, int idx) {
	{ strategy.info() } -> std::convertible_to<std::string>;
	{ strategy.interrupt(idx) };
};
