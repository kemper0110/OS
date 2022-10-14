#include "SharedBattleship.h"


//void client() {
//
//	auto hwnd = OpenFileMapping(
//		FILE_MAP_ALL_ACCESS,   // read/write access
//		FALSE,                 // do not inherit the name
//		L"Global/Object");               // name of mapping object
//
//	auto shared = MapViewOfFile(hwnd, // handle to map object
//		FILE_MAP_ALL_ACCESS,  // read/write permission
//		0,
//		0,
//		256);
//}

SharedBattleship::SharedBattleship()
{
	//event = CreateEvent(NULL, FALSE, FALSE, L"BattleshipEvent");
	//if (not event) {
	//	std::cout << "Event error: ";
	//	perror(GetLastError());
	//	std::exit(-1);
	//}
	_filemapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof SharedState, L"BattleshipSharedState");
	const auto code = GetLastError();
	std::cout << (code == ERROR_ALREADY_EXISTS) << '\n';
	if (code == ERROR_ALREADY_EXISTS)
		player = 1;
	else
		player = 0;
	if (not _filemapping) {
		std::cout << "mapping error: ";
		perror(code);
		std::exit(1);
	}
	else
		std::cout << "created mapping\n";
	_shared = MapViewOfFile(_filemapping, FILE_MAP_ALL_ACCESS, 0, 0, sizeof SharedState);
	if (not _shared) {
		std::cout << "view error: ";
		perror(GetLastError());
		std::exit(2);
	}

	const auto s_ptr = reinterpret_cast<SharedState*>(_shared);
	if (player == 0)
		shared = std::construct_at(s_ptr);
	else
		shared = s_ptr;
	std::cout << shared << '\n';
}

SharedBattleship::~SharedBattleship()
{
	UnmapViewOfFile(_shared);
	CloseHandle(_filemapping);
}

bool SharedBattleship::waitBeginning()
{
	std::cout << shared->startup << '\n';
	if (shared->startup == 0)
	{
		shared->startup = 1;
		while (shared->startup != 0);
		return true;
	}
	else {
		shared->startup = 0;
		return false;
	}
}

void SharedBattleship::send(const Message& message)
{
	std::cout << "sending\n";

	//shared->sending.acquire();

	//shared->pending[!player].acquire();

	// пока не прочитали, не отправляем
	shared->received[!player].acquire();

	//shared->sended[player].acquire();

	shared->message_mtx.lock();
	shared->message = message;
	shared->message_mtx.unlock();


	//shared->sending.release();
	//if (SetEvent(event) == 0) {
	//	std::cout << "set event error: ";
	//	perror(GetLastError());
	//	std::exit(1);
	//}
}

SharedBattleship::Message SharedBattleship::receive()
{
	std::cout << "receiving\n";

	//shared->receiving.acquire();

	//shared->pending[player].release();

	// пока не отправили, не читаем
	shared->sended[player].acquire();

	shared->message_mtx.lock();
	const auto m = shared->message;
	shared->message_mtx.unlock();

	//shared->received[player].release();


	//shared->receiving.release();

	//while(shared->receiving)
	//const auto wait_status = WaitForSingleObject(event, INFINITE);
	//if (wait_status != WAIT_OBJECT_0) {
	//	std::cout << "wait event error: ";
	//	perror(GetLastError());
	//	std::exit(1);
	//}
	return m;
}