#pragma once
#include <optional>
#include <fstream>


template<typename T>
std::optional<T> read(std::wifstream& ifs) {
	T value;
	const auto isOk = static_cast<bool>(ifs >> value);
	if (isOk) return value;
	return std::nullopt;
}


