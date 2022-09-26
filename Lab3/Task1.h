#pragma once

#define LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <optional>
#include <filesystem>


#include "Reader.h"


struct Task1
{
	struct Task {
		std::wstring name;
		int time;
	};
	int run();
};


template<>
std::optional<Task1::Task> read(std::wifstream& ifs);



