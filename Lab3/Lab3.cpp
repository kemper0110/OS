#define LEAN_AND_MEAN
#include <Windows.h>
#include <vector>
#include <string>
#include <iostream>

int main()
{
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	if (CreateProcess(L"C:\\Windows\\explorer.exe", NULL, NULL, NULL,
		FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi))
	{
		CloseHandle(pi.hThread);
		DWORD state = 0;
		while (state == 0) {
			state = WaitForSingleObject(pi.hProcess, 5 * 1000);
		}
		if (state == WAIT_TIMEOUT)
		{
			TerminateProcess(pi.hProcess, 0);
		}
		CloseHandle(pi.hProcess);
	}


	//STARTUPINFO si;
	//PROCESS_INFORMATION pi;

	//ZeroMemory(&si, sizeof(si));
	//si.cb = sizeof(si);
	//ZeroMemory(&pi, sizeof(pi));

	//if (!CreateProcessW(L"C:/Windows/explorer.exe",   // No module name (use command line)
	//	NULL,        // Command line
	//	NULL,           // Process handle not inheritable
	//	NULL,           // Thread handle not inheritable
	//	FALSE,          // Set handle inheritance to FALSE
	//	0,              // No creation flags
	//	NULL,           // Use parent's environment block
	//	NULL,           // Use parent's starting directory 
	//	&si,            // Pointer to STARTUPINFO structure
	//	&pi)           // Pointer to PROCESS_INFORMATION structure
	//	)
	//{
	//	printf("CreateProcess failed (%d).\n", GetLastError());
	//	return -1;
	//}


	//CloseHandle(pi.hThread);
	//std::cout << WaitForSingleObject(pi.hProcess, 5000);
	//CloseHandle(pi.hProcess);


	//std::vector<std::wstring> processes{
	//	L"explorer.exe",
	//	L"cmd.exe"
	//};
	//std::vector<int> max_time{ 5, 2 };

	//for (int i = 0; i < processes.size(); ++i) {
	//	const auto& process = processes[i];

	//	STARTUPINFO si = { 
	//		.cb = sizeof(si)
	//	};
	//	PROCESS_INFORMATION pi{};
	//	const auto status = CreateProcess(process.c_str(),
	//		NULL, NULL, NULL, FALSE,
	//		NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	//	if (status == 0) {
	//		std::cout << "process created\n";
	//		//const auto wait_status = WaitForSingleObject(pi.hProcess, max_time[i] * 1000);
	//		//if (wait_status == WAIT_TIMEOUT) {
	//		//	TerminateProcess(pi.hProcess, 0);
	//		//	std::cout << "Force terminated\n";
	//		//}
	//		//else if (wait_status == WAIT_ABANDONED) {
	//		//	std::cout << "wait abandoned\n";
	//		//}
	//		//else if (wait_status == WAIT_OBJECT_0) {
	//		//	std::cout << "Wait obj 0\n";
	//		//}
	//		//else if (wait_status == WAIT_FAILED) {
	//		//	std::cout << "Failed\n";
	//		//}
	//		//else {
	//		//	std::cout << "Gracefully shutdown\n";
	//		//}
	//		std::cin.get();
	//		TerminateProcess(pi.hProcess, 0);
	//		CloseHandle(pi.hThread);
	//		CloseHandle(pi.hProcess);
	//	}
	//	else {
	//		std::cout << "process not created\n";
	//		std::cout << "error: " << GetLastError() << '\n';
	//	}


	//}

	//const auto handle = OpenEvent(EVENT_MODIFY_STATE, FALSE, L"C:/Windows/explorer.exe");
	//if (handle == 0)
	//	return -1;
	//const auto wait_status = WaitForSingleObject(handle, 1000 * 6);
	//std::cout << wait_status << " ? " << WAIT_FAILED << '\n';
}
