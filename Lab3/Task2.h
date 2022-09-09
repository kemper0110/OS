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
/*
		Реализовать программу, которая запускает исполняемые
		файлы (исполняемыми считаются файлы с расширениями .exe,
		.bat, .cmd) из указанного в качестве параметра каталога. После
		завершения каждого запущенного процесса соответствующий
		исполняемый файл должен удаляться. В случае если в указанном
		каталоге отсутствуют файлы, программа должна ожидать их
		появления. Учесть, что запуск файлов с расширениями .bat и
		.cmd может быть осуществлен только с помощью командного
		процессора cmd.exe
*/
struct Task2
{
	std::vector<std::string_view> args;
	Task2(std::vector<std::string_view>&& args) : args(std::move(args)) {}

	int run();
};

