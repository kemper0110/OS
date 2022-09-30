#include "Task3.h"


// run with ping
int Task3::run() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	std::system("chcp 866 && cls");
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
		0,
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

	auto cmd = (std::wstringstream() << path.data()).str();

	const auto inherit = TRUE;
	const auto createStatus = CreateProcess(NULL, cmd.data(), NULL, NULL, inherit, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	if (createStatus == 0) {
		std::cout << getError() << '\n';
		return -1;
	}
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return 0;
}