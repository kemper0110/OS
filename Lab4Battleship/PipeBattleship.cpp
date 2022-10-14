#include "PipeBattleship.h"
#include <iostream>

PipeBattleship::PipeBattleship()
{
	pipe = CreateNamedPipe(LR"!!!(\\.\pipe\lab4Battleship)!!!",
		PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, PIPE_UNLIMITED_INSTANCES, 512, 512, 5000, NULL);
	const auto code = GetLastError();
	std::cout << (code == ERROR_ALREADY_EXISTS) << '\n';
	if (code == ERROR_ALREADY_EXISTS) {
		player = 1;
		//const auto wait_status = WaitNamedPipe(LR"!!!(\\.\pipe\lab4Battleship)!!!", NMPWAIT_WAIT_FOREVER);
		pipe = CreateFile(LR"!!!(\\.\pipe\lab4Battleship)!!!", GENERIC_ALL, NULL, NULL, OPEN_EXISTING, NULL, NULL);
		if (pipe == INVALID_HANDLE_VALUE) {
			std::cout << "write error: " << GetLastError();
			std::exit(1);
		}
	}
	else
		player = 0;
	std::cout << "player: " << player << '\n';
	if (pipe == INVALID_HANDLE_VALUE) {
		std::cout << "pipe error: ";
		perror(GetLastError());
		std::cout << std::endl;
		std::exit(1);
	}
}

bool PipeBattleship::waitBeginning()
{
	if (player == 0) {
		if (not ConnectNamedPipe(pipe, NULL)) {
			std::cout << "connect error: " << GetLastError();
			CloseHandle(pipe);
			std::exit(2);
		}
	}
	return player == 0;
}

void PipeBattleship::send(const Message& message)
{
	DWORD written;
	const auto write_status = WriteFile(pipe, &message, sizeof message, &written, NULL);
	if (not write_status) {
		std::cout << "write error: ";
		perror(GetLastError());
		std::exit(3);
	}
}

PipeBattleship::Message PipeBattleship::receive()
{
	DWORD cbRead;
	Message message;
	const auto read_successfully = ReadFile(pipe, &message, sizeof message, &cbRead, NULL);
	if (not read_successfully || cbRead != sizeof message) {
		const auto code = GetLastError();
		if (code == ERROR_BROKEN_PIPE) {
			std::cout << "client disconnected\n";
			std::exit(1);
		}
		std::cout << "read error: ";
		perror(code);
	}
	return message;
}
