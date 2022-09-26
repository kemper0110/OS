#define LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>


template<class Lambda>
struct defer : Lambda {
	~defer() { Lambda::operator()(); }
};


int main()
{
	std::system("chcp 65001 && cls");

	const auto mutex = CreateMutex(NULL, FALSE, L"Lab4ConsoleMutex");
	defer d{
		[mutex] {
			CloseHandle(mutex);
			std::cout << "mutex closed\n";
		}
	};

	if (mutex == NULL) {
		std::cout << "Mutex error: " << GetLastError();
		std::exit(-1);
	}

	std::wifstream ifs("text.txt");
	std::wstring str;
	while (std::getline(ifs, str)) {
		// lock
		const auto wait_status = WaitForSingleObject(mutex, INFINITE);
		if (wait_status == WAIT_OBJECT_0) {
			std::wcout << str << '\n';
			Sleep(500);
			// unlock
			ReleaseMutex(mutex);
		}
		else
			std::cout << "error\n";
	}
}


//std::cout << "Can't release mutex: " << GetLastError();
//std::exit(-1);
//}