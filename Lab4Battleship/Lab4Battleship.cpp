#define LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <random>
#include <array>

void perror(int code) {
	LPSTR messageBuffer = nullptr;

	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
	std::cout << std::string_view(messageBuffer, size) << '\n';
	LocalFree(messageBuffer);
}


int main()
{
	enum Cell {
		Empty = ' ', Ship = 'T', FiredEmpty = 'O', FiredShip = 'X'
	};


	std::array<std::array<Cell, 5>, 5> Field{
		Empty, Ship, Empty, Ship, Empty,
		Empty, Empty, Ship, Empty, Ship,
		Empty, Ship, Empty, Ship, Empty,
		Empty, Empty, Ship, Empty, Ship,
		Empty, Ship, Empty, Ship, Empty,
	};

	constexpr auto buff_size = 256;

	const auto map_file = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, buff_size, L"Global/Object");
	if (not map_file) {
		std::cout << "mapping error: ";
		perror(GetLastError());
		std::exit(1);
	}
	const auto shared = MapViewOfFile(map_file, FILE_MAP_ALL_ACCESS, 0, 0, buff_size);
	if (not shared) {
		std::cout << "view error: ";
		perror(GetLastError());
		std::exit(2);
	}
	const auto ptr = (int*)shared;
	std::cout << ptr[0] << '\n';

	ptr[0] = std::random_device()();
	
	Sleep(INFINITE);

	UnmapViewOfFile(shared);
	CloseHandle(map_file);
}

void client() {

	auto hwnd = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		L"Global/Object");               // name of mapping object

	auto shared = MapViewOfFile(hwnd, // handle to map object
		FILE_MAP_ALL_ACCESS,  // read/write permission
		0,
		0,
		256);
}
