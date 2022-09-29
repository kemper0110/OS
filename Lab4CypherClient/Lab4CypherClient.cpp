#include <iostream>
#define LEAN_AND_MEAN
#include <Windows.h>

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
	const auto pipe_name = LR"!!!(\\.\pipe\lab4cypherserver)!!!";


	const auto wait_status = WaitNamedPipe(pipe_name, NMPWAIT_WAIT_FOREVER);
	//if (not wait_status) {
	//	std::cout << "wait error: ";
	//	perror(GetLastError());
	//	std::exit(-1);
	//}
	const auto pipe = CreateFile(pipe_name, GENERIC_ALL, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (pipe == INVALID_HANDLE_VALUE) {
		std::cout << "write error: " << GetLastError();
		std::exit(1);
	}


	std::string input;
	std::cin >> input;
	DWORD written;
	const auto write_status = WriteFile(pipe, input.c_str(), input.size() * sizeof(input[0]), &written, FALSE);
	if (not write_status) {
		std::cout << "write error: " << GetLastError();
		std::exit(2);
	}


	char buffer[1024];
	DWORD read;
	const auto read_status = ReadFile(pipe, buffer, sizeof(buffer), &read, NULL);
	if (not read_status) {
		std::cout << "write error: " << GetLastError();
		std::exit(3);
	}
	std::cout << std::string_view(buffer, read) << '\n';

	CloseHandle(pipe);
}
