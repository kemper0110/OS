#include "Task2.h"

// run with C:\Users\danil\source\repos\OS\Lab3\target
int Task2::run() {
	if (args.size() != 2)
		return -1;

	const auto path = args[1];

	while (1) {
		for (const auto& entry : fs::directory_iterator(fs::path(path))) {
			if (entry.is_directory())
				continue;
			const auto filepath = entry.path();
			std::wcout << "trying: " << filepath << '\n';
			const auto ext = filepath.extension();

			if (ext != "bat" && ext != "exe" && ext != "cmd")
				continue;

			std::cout << std::endl;	// flush
			auto str = filepath.string();
			auto quoted = "\"" + str + "\"";
			std::system(str.c_str());

			std::remove(filepath.string().c_str());

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
		}

		// on windows there is now inotify so go to sleep
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}


	return 0;
}

