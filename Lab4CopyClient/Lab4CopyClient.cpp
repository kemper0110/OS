#define LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>

void perror(int code) {
	LPSTR messageBuffer = nullptr;

	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
	std::cout << std::string_view(messageBuffer, size) << '\n';
	LocalFree(messageBuffer);
}

int main()
{
	const auto pipe_name = LR"!!!(\\.\pipe\lab4copyserver)!!!";


	WaitNamedPipe(pipe_name, INFINITE);
	const auto pipe = CreateFile(pipe_name, GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, NULL, NULL);

	if (pipe == INVALID_HANDLE_VALUE) {
		std::cout << "write error: " << GetLastError();
		std::exit(1);
	}


	std::cout << "intput = src;dst\n";
	std::string message;
	std::cin >> message;
	DWORD written;
	const auto write_status = WriteFile(pipe, message.c_str(), message.size() * sizeof(message[0]), &written, FALSE);
	if (not write_status) {
		std::cout << "write error: " << GetLastError();
		std::exit(2);
	}


	CloseHandle(pipe);
}
