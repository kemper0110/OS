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
		����������� ���������, ������� ��������� �����������
		����� (������������ ��������� ����� � ������������ .exe,
		.bat, .cmd) �� ���������� � �������� ��������� ��������. �����
		���������� ������� ����������� �������� ���������������
		����������� ���� ������ ���������. � ������ ���� � ���������
		�������� ����������� �����, ��������� ������ ������� ��
		���������. ������, ��� ������ ������ � ������������ .bat �
		.cmd ����� ���� ����������� ������ � ������� ����������
		���������� cmd.exe
*/
struct Task2
{
	std::vector<std::string_view> args;
	Task2(std::vector<std::string_view>&& args) : args(std::move(args)) {}

	int run();
};

