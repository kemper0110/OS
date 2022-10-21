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
	receivedEvent = CreateEvent(NULL, TRUE, FALSE, L"BattleshipReceivedEvent");
	if (not receivedEvent) {
		std::cout << "received evt error: ";
		perror(GetLastError());
		std::exit(-1);
	}
	sendedEvent = CreateEvent(NULL, FALSE, FALSE, L"BattleshipSendedEvent");
	if (not sendedEvent) {
		std::cout << "sended evt error: ";
		perror(GetLastError());
		std::exit(-1);
	}
	//event = CreateEvent(NULL, FALSE, FALSE, L"BattleshipEvent");

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
	if (player == 0) {
		shared = std::construct_at(s_ptr);
		//shared->pending[0].acquire();
	}
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

// интерфейс - какой функционал реализует класс
// стандарт на взаимодействие двух и более частей
// с чем совместим и какие операции

void SharedBattleship::send(const Message& message)
{
	std::cout << "sending\n";
	//shared->sending.acquire();
	//shared->pending[!player].acquire();
	//shared->received[!player].acquire();
	//shared->sended[player].acquire();


	// пока флаг оппонента выставлен, не выставляем его флаг
	
	// atomic 
	//while (shared->pending[!player]);
	// sema
	shared->pending[!player].acquire();

	// когда флаг сброшен, пишем
	shared->message_mtx.lock();
	shared->message = message;
	shared->message_mtx.unlock();

	// когда написали, выставляем флаг
	// atomic
	//shared->pending[!player] = true;
	// sema
	shared->pending[!player].release();

	/*if (WaitForSingleObject(receivedEvent, INFINITE) != WAIT_OBJECT_0) {
	std::cout << "wait event error: ";
	perror(GetLastError());
	std::exit(1);
	}*/


	//ResetEvent(receivedEvent);
	//SetEvent(sendedEvent);
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



	// пока флаг не выставили, ожидаем
	// atomic
	//while (not shared->pending[player]);
	// sema
	shared->pending[player].acquire();
	
	// когда выставили, читаем

	shared->message_mtx.lock();
	const auto m = shared->message;
	shared->message_mtx.unlock();

	// когда прочитали, сбрасываем флаг
	// atomic
	//shared->pending[player] = false;
	// semaphore
	shared->pending[player].release();

	//shared->receiving.acquire();
//shared->pending[player].release();
//shared->sended[player].acquire();

//if (WaitForSingleObject(sendedEvent, INFINITE) != WAIT_OBJECT_0) {
//	std::cout << "wait event error: ";
//	perror(GetLastError());
//	std::exit(1);
//}
	//ResetEvent(sendedEvent);
	//SetEvent(receivedEvent);

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