#pragma once
/*
		Реализовать программу, которая запускает исполняемые
		файлы (исполняемыми считаются файлы с расширениями .exe,
		.bat, .cmd) из указанного в качестве параметра каталога. После
		завершения каждого запущенного процесса соответствующий
		исполняемый файл должен удаляться. В случае если в указанном
		каталоге отсутствуют файлы, программа должна ожидать их
		появления. Учесть, что запуск файлов с расширениями .bat и
		.cmd может быть осуществлен только с помощью командного
		процессора cmd.exe
*/
class Task2
{
	static std::string onError() {
		const auto errorMessageID = GetLastError();

		LPSTR messageBuffer = nullptr;

		//Ask Win32 to give us the string version of that message ID.
		//The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

		//Copy the error message into a std::string.
		std::string message(messageBuffer, size);

		//Free the Win32's string's buffer.
		LocalFree(messageBuffer);
		return message;
	}
public:
	std::vector<std::string_view> args;
	Task2(std::vector<std::string_view>&& args) : args(std::move(args)) {}

	int run() {
		if (args.size() != 2)
			return -1;

		const auto path = args[1];

		while (1) {

			for (const auto& entry : fs::directory_iterator(fs::path(path))) {
				if (entry.is_directory())
					continue;
				const auto filepath = entry.path();
				std::cout << "running: " << filepath << '\n';
				const auto ext = filepath.extension();

				STARTUPINFO si = { .cb = sizeof(si) };
				PROCESS_INFORMATION pi{};
				[&] {
					if (ext == "exe") {
						return std::optional<int>{
							CreateProcess(filepath.c_str(), NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi)
						};
					}
					if (ext == "bat" or ext == "cmd") {
						std::wstring str = L"cmd.exe /C ";
						str += filepath.c_str();
						return std::optional<int>{
							CreateProcess(NULL, str.data(), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi)
						};
					}
					return std::optional<int>{ std::nullopt };
				}().transform([&](int createStatus) {
					if (createStatus == 0) {
						std::cout << onError() << '\n';
						return 0;
					}
					//const auto waitStatus = WaitForSingleObject(pi.hProcess, INFINITE);
					//if (waitStatus == WAIT_FAILED)
						//std::cout << onError() << '\n';
					});
			}

			// on windows there is now inotify so go to sleep
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}




		return 0;
	}
};

