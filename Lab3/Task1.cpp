#include "Task1.h"

#include "WinApiEz.h"

//extern template void read<Task1::Task>(std::ifstream&);



template<>
std::optional<Task1::Task> read(std::ifstream& ifs) {
	return read<std::string>(ifs).and_then(
		[&ifs](std::string&& str) {
			return read<int>(ifs).transform(
				[&str](int time) {
					return Task1::Task{ std::move(str), time };
				});
		});
};

int Task1::run() {

	std::vector<Task> tasks;
	{
		std::ifstream ifs("config.txt");
		for (auto task = read<Task>(ifs); 
			task.has_value(); 
			task = read<Task>(ifs))
			tasks.push_back(std::move(task.value()));
	};



	for (const auto& task : tasks) {
		std::cout << task.name << " in " << task.time << " : ";

		STARTUPINFOA si = { .cb = sizeof(si) };
		PROCESS_INFORMATION pi{};
		const auto createStatus = CreateProcessA(task.name.c_str(), NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
		if (createStatus == 0) {
			std::cout << getError() << '\n';
			continue;
		}
		const auto waitStatus = WaitForSingleObject(pi.hProcess, task.time * 1000);
		switch (waitStatus) {
		case WAIT_OBJECT_0:
			std::cout << "process got in time\n";
			break;
		case WAIT_TIMEOUT:
			std::cout << "process got timeout\n";
			break;
		case WAIT_FAILED:
			std::cout << getError() << '\n';
			continue;
		}
		TerminateProcess(pi.hProcess, 0);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}

	return 0;
}
