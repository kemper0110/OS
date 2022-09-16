#define LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <optional>
#include <filesystem>

namespace fs = std::filesystem;


#include "Task1.h"
#include "Task2.h"
#include "Task3.h"

std::vector<std::string_view> normalize(int argc, char** argv) {
	std::vector<std::string_view> args;
	for (int i = 0; i < argc; ++i)
		args.emplace_back(argv[i]);
	return args;
}

int main(int argc, char** argv)
{
	std::system("chcp 1251 && cls");
	//return Task1().run();
	//return Task2(normalize(argc, argv)).run();
	return Task3(normalize(argc, argv)).run();
}
