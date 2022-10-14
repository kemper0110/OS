#include "Task3.h"


// run with C:\Windows\System32\ipconfig.exe
// C:\Users\danil\source\repos\kemper0110\OS\Lab3\dummy_prog.exe
int Task3::run() {
	if (args.size() != 2)
		return -1;
	const auto path = args[1];

	SECURITY_ATTRIBUTES sa{
		.nLength = sizeof(sa),
		.lpSecurityDescriptor = NULL,
		.bInheritHandle = TRUE
	};

	const auto output_handle = CreateFile(L"output.txt",
		FILE_APPEND_DATA,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		&sa,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	const auto input_handle = CreateFile(L"input.txt",
		FILE_READ_DATA,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		&sa,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);


	STARTUPINFO si = {
		.cb = sizeof(si),
		.dwFlags = STARTF_USESTDHANDLES,
		.hStdInput = input_handle,
		.hStdOutput = output_handle,
		.hStdError = output_handle
	};
	PROCESS_INFORMATION pi{};

	const auto wstr = (std::wstringstream() << path.data()).str();


	const auto inherit = TRUE;
	const auto createStatus = CreateProcess(wstr.data(), NULL, NULL, NULL, inherit, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	if (not createStatus) {
		std::cout << getError() << '\n';
		return -1;
	}
	const auto waitStatus = WaitForSingleObject(pi.hProcess, INFINITE);
	if (waitStatus == WAIT_FAILED) {
		std::cout << getError() << '\n';
		return -1;
	}
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return 0;
}