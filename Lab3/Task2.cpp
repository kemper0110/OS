#include "Task2.h"

// run with 
// C:\Users\danil\source\repos\kemper0110\OS\Lab3\target
int Task2::run() {
	if (args.size() != 2)
		return -1;

	const auto path = args[1];

	const auto run_all = [&path] {
		for (const auto& entry : fs::directory_iterator(fs::path(path))) {
			if (entry.is_directory())
				continue;
			const auto filepath = entry.path();
			std::cout << "\ntrying: " << filepath.string() << '\n';

			const auto ext = filepath.extension();


			STARTUPINFO si = { .cb = sizeof(si) };
			PROCESS_INFORMATION pi{};
			BOOL createStatus = true;
			if (ext == ".bat" || ext == ".cmd") {
				auto str = L"cmd.exe /C " + filepath.wstring();
				createStatus = CreateProcess(NULL, str.data(), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
			}
			else if (ext == ".exe") {
				createStatus = CreateProcess(filepath.wstring().c_str(), NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
			}
			else continue;

			if (createStatus == 0) {
				std::cout << "error: " << getError() << '\n';
				continue;
			}
			const auto waitStatus = WaitForSingleObject(pi.hProcess, INFINITE);
			switch (waitStatus) {
			case WAIT_OBJECT_0:
				std::cout << "process done\n";
				break;
			case WAIT_FAILED:
				std::cout << "error: " << getError() << '\n';
				continue;
			}
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);

			fs::remove(filepath);
			
			std::cout << "Executed and removed\n\n";
		}
	};

	run_all();


	const auto handle = FindFirstChangeNotificationA(args[1].data(), FALSE, FILE_NOTIFY_CHANGE_FILE_NAME);
	if (INVALID_HANDLE_VALUE == handle) {
		std::cout << "FindFirstChangeNotification function failed\n";
		std::exit(GetLastError());
	}


	while (true) {
		std::cout << "waiting notifications\n";

		const auto wait_status = WaitForSingleObject(handle, INFINITE);

		switch (wait_status) {
		case WAIT_OBJECT_0:
			run_all();

			if (FindNextChangeNotification(handle) == FALSE) {
				std::cout << "FindNextChangeNotification error\n";
				std::exit(GetLastError());
			}
			break;
		default:
			std::cout << "wait error\n";
			std::exit(GetLastError());
		}
	}
		


			//STARTUPINFO si = { .cb = sizeof(si) };
			//PROCESS_INFORMATION pi{};
			//BOOL createStatus;
			//if (ext == "exe") {
			//	std::cout << "executing: " << filepath << '\n';
			//	createStatus = CreateProcess(filepath.c_str(), NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
			//} 
			//else if (ext == "bat" or ext == "cmd") {
			//	std::wstring command = L"cmd.exe /C ";
			//	command += filepath.c_str();
			//	std::wcout << "executing: " << command << '\n';
			//	createStatus = CreateProcess(NULL, command.data(), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
			//}
			//else
			//	continue;
			//if (createStatus == 0)
			//	std::cout << getError() << '\n';
			//WaitForSingleObject(pi.hProcess, INFINITE);
			//CloseHandle(pi.hThread);
			//CloseHandle(pi.hProcess);
		//}
	//	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	return 0;
}

