#define LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <string_view>
#include <filesystem>


namespace fs = std::filesystem;

/*
		Ќаписать процесс, осуществл€ющий копирование файлов (сервер
		копировани€). »мена исходного файла и файла-назначени€
		передаютс€ этому процессу через именованный канал. –еализовать
		клиент, позвол€ющий передавать задани€ на сервер копировани€.
*/

void perror(int code) {
	LPSTR messageBuffer = nullptr;

	//Ask Win32 to give us the string version of that message ID.
	//The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
	std::cout << std::string_view(messageBuffer, size) << '\n';
	LocalFree(messageBuffer);
}

int main() {
	std::system("chcp 1251 && cls");

	while (1) {
		const auto pipe = CreateNamedPipe(LR"!!!(\\.\pipe\lab4copyserver)!!!",
			PIPE_ACCESS_INBOUND, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, PIPE_UNLIMITED_INSTANCES, 512, 512, 5000, NULL);

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
			const auto delim = input.find(';');
			if (delim == std::string_view::npos) {
				std::cout << "required 2 pathes with `;` delim\n";
				break;
			}
			const auto src = fs::path(input.substr(0, delim));
			std::cout << "src: " << src << '\n';
			if (not fs::exists(src)) {
				std::cout << src << " - not exists\n";
				break;
			}
			const auto dst = fs::path(input.substr(delim + 1));
			std::cout << "dst: " << dst << '\n';
			fs::copy_file(src, dst);
		}
	}

}