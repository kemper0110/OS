#define LEAN_AND_MEAN
#include <Windows.h>

#include <iostream>

int main()
{
	const auto mutex = CreateMutex(NULL, FALSE, NULL);
	if (mutex == NULL) {
		std::cout << "Mutex error: " << GetLastError();
		exit(-1);
	}

	const auto wait_status = WaitForSingleObject(mutex, INFINITE);

	switch (wait_status) {
	case WAIT_OBJECT_0:
		std::cout << "X\n";
		// do something
		if (!ReleaseMutex(mutex)) {
			std::cout << "Can't release mutex: " << GetLastError();
			exit(-1);
		}
		break;
	case WAIT_ABANDONED:
		// mutex has been closed
		std::cout << "Mutex error: " << GetLastError();
		exit(-1);
	}

	ReleaseMutex(mutex);

	CloseHandle(mutex);

}
