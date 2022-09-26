#define LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>

template<class Lambda>
struct defer : Lambda {
	~defer() { Lambda::operator()(); }
};


int main()
{
	std::system("chcp 65001 && cls");
	const auto event = CreateEvent(NULL, TRUE, FALSE, L"Lab4Signal");
	if (event == NULL) {
		std::cout << "Mutex error: " << GetLastError();
		std::exit(-1);
	}
	defer d{ [event] { CloseHandle(event); } };


	std::cout << "any key to signal\n";
	std::cin.get();
	if (SetEvent(event) == 0) {
		std::cout << "error\n";
		std::exit(-1);
	}
}
