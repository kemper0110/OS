#define LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <ranges>
#include <algorithm>
#include <string_view>

namespace ranges = std::ranges;


void perror(int code) {
	LPSTR messageBuffer = nullptr;

	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
	std::cout << std::string_view(messageBuffer, size) << '\n';
	LocalFree(messageBuffer);
}

int main()
{
	std::system("chcp 1251 && cls");



	while (1) {
		const auto pipe = CreateNamedPipe(LR"!!!(\\.\pipe\lab4cypherserver)!!!",
			PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE , PIPE_UNLIMITED_INSTANCES, 512, 512, 5000, NULL);

		if (pipe == INVALID_HANDLE_VALUE) {
			std::cout << "pipe error: ";
			perror(GetLastError());
			std::cout << std::endl;
			std::exit(1);
		}

		if (not ConnectNamedPipe(pipe, NULL)) {
			std::cout << "connect error: " << GetLastError();
			CloseHandle(pipe);
			std::exit(2);
		}

		while (1) {
			DWORD cbRead;
			char buff[4096];
			const auto read_successfully = ReadFile(pipe, buff, sizeof(buff), &cbRead, NULL);
			if (not read_successfully || cbRead == 0) {
				const auto code = GetLastError();
				if (code == ERROR_BROKEN_PIPE) {
					std::cout << "client disconnected\n";
					break;
				}
				std::cout << "read error: ";
				perror(code);
				break;
			}
			const auto input = std::string_view(buff, cbRead);
			std::cout << input << '\n';
			
			std::string transformed;
			transformed.reserve(input.size());
			ranges::transform(input, std::back_inserter(transformed), [](char c) {
				if (c >= 'A' && c <= 'Z') 
					return static_cast<char>('Z' - (c - 'A'));
				if (c >= 'a' && c <= 'z') 
					return static_cast<char>('z' - (c - 'a'));
				return c;
				}
			);
			std::cout << transformed << '\n';

			DWORD written;
			const auto write_status = WriteFile(pipe, transformed.c_str(), transformed.size() * sizeof transformed[0], &written, NULL);
			if (not write_status) {
				std::cout << "write error: ";
				perror(GetLastError());
				std::exit(3);
			}
		}
	}
}
