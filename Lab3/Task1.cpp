#include "Task1.h"

#include "WinApiEz.h"


template<>
std::optional<Task1::Task> read(std::wifstream& ifs) {
	return read<std::wstring>(ifs).and_then(
		[&ifs](std::wstring&& str) {
			return read<int>(ifs).transform(
				[&str](int time) {
					return Task1::Task{ std::move(str), time };
				});
		});
};

int Task1::run() {
	std::vector<Task> tasks;
	{
		std::wifstream ifs("config.txt");
		for (auto task = read<Task>(ifs); 
			task.has_value(); 
			task = read<Task>(ifs))
			tasks.push_back(std::move(task.value()));
	};



	for (const auto& task : tasks) {
		std::wcout << task.name << " in " << task.time << " : ";

		STARTUPINFO si = { .cb = sizeof(si) };
		PROCESS_INFORMATION pi{};
		const auto createStatus = CreateProcess(task.name.c_str(), NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
		if (createStatus == 0) {
			std::cout << getError() << '\n';
			continue;
		}
		const auto waitStatus = WaitForSingleObject(pi.hProcess, task.time * 1000);
		switch (waitStatus) {
		case WAIT_OBJECT_0:
			std::cout << "процесс уложился во времени\n";
			break;
		case WAIT_TIMEOUT:
			std::cout << "процесс не уложился во времени\n";
			break;
		case WAIT_FAILED:
			std::cout << getError() << '\n';
			break;
		}
		TerminateProcess(pi.hProcess, 0);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}

	return 0;
}
