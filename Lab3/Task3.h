#pragma once
#define LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <optional>
#include <filesystem>

namespace fs = std::filesystem;

#include "WinApiEz.h"


struct Task3
{
	std::vector<std::string_view> args;
	Task3(std::vector<std::string_view>&& args) : args(std::move(args)) {}

	int run();
};

