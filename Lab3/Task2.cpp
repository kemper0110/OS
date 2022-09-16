#include "Task2.h"

// run with C:\Users\danil\source\repos\OS\Lab3\target
int Task2::run() {
	if (args.size() != 2)
		return -1;

	const auto path = args[1];
	/*
		#define FILE_NOTIFY_CHANGE_FILE_NAME    0x00000001   
		#define FILE_NOTIFY_CHANGE_DIR_NAME     0x00000002   
		#define FILE_NOTIFY_CHANGE_ATTRIBUTES   0x00000004   
		#define FILE_NOTIFY_CHANGE_SIZE         0x00000008   
		#define FILE_NOTIFY_CHANGE_LAST_WRITE   0x00000010   
		#define FILE_NOTIFY_CHANGE_LAST_ACCESS  0x00000020   
		#define FILE_NOTIFY_CHANGE_CREATION     0x00000040   
		#define FILE_NOTIFY_CHANGE_SECURITY     0x00000100  
	*/

	const auto run_all = [&path] {
		for (const auto& entry : fs::directory_iterator(fs::path(path))) {
			if (entry.is_directory())
				continue;
			const auto filepath = entry.path();
			std::cout << "running: " << filepath.string() << '\n';
			const auto ext = filepath.extension();

			//std::cout << ext << '\n';
			const auto isExecutable = ext == ".bat" || ext == ".exe" || ext == ".cmd";
			if (not isExecutable)
				continue;

			std::cout << std::endl;	// flush
			auto quoted = "\"" + filepath.string() + "\"";
			//std::cout << "system: " << quoted << '\n';
			std::system(quoted.c_str());
			fs::remove(filepath);
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

