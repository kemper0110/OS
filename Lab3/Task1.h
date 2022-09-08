#pragma once


class Task1
{
	struct Task {
		std::string name;
		int time;
	};

	template<typename T>
	static std::optional<T> read(std::ifstream& ifs) {
		T value;
		const auto isOk = static_cast<bool>(ifs >> value);
		if (isOk) return value;
		return std::nullopt;
	}
	template<>
	static std::optional<Task> read(std::ifstream& ifs) {
		return read<std::string>(ifs).and_then(
			[&ifs](std::string&& str) {
				return read<int>(ifs).transform(
					[&str](int time) {
						return Task{ std::move(str), time };
					});
			});
	}

public:
	Task1() {

	}

	std::string onError() {
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

	int run() {

		std::vector<Task> tasks;
		{
			std::ifstream ifs("config.txt");
			while (1) {
				auto optional_task = read<Task>(ifs);
				if (optional_task.has_value())
					tasks.push_back(std::move(optional_task.value()));
				else break;
			}
		};



		for (const auto& task : tasks) {
			STARTUPINFOA si = { .cb = sizeof(si) };
			PROCESS_INFORMATION pi{};
			const auto createStatus = CreateProcessA(task.name.c_str(), NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
			if(createStatus == 0){
				std::cout << onError() << '\n';
				continue;
			}
			const auto state = WaitForSingleObject(pi.hProcess, task.time * 1000);
			switch (state) {
			case WAIT_OBJECT_0:
				std::cout << "process got in time\n";
				break;
			case WAIT_TIMEOUT:
				std::cout << "process got timeout\n";
				break;
			case WAIT_FAILED:
				std::cout << onError() << '\n';
				continue;
			}
			TerminateProcess(pi.hProcess, 0);
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
		}

		return 0;
	}
};

