#define LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>


int main()
{
	const auto pipe_name = LR"!!!(\\.\pipe\lab4copyserver)!!!";


	WaitNamedPipe(pipe_name, INFINITE);
	const auto pipe = CreateFile(pipe_name, GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, NULL, NULL);

	if (pipe == INVALID_HANDLE_VALUE) {
		std::cout << "write error: " << GetLastError();
		std::exit(1);
	}


	while (1) {
		std::wstring message;
		std::wcin >> message;
		DWORD written;
		const auto write_status = WriteFile(pipe, message.c_str(), message.size() * sizeof(message[0]), &written, FALSE);
		if (not write_status) {
			std::cout << "write error: " << GetLastError();
			std::exit(2);
		}
	}


	CloseHandle(pipe);
}
