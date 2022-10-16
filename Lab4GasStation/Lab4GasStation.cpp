#include <functional>
#include <iomanip>
#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <cassert>

#include <sdkddkver.h>
#include <boost/asio.hpp>

namespace asio = boost::asio;
using asio::use_awaitable, asio::detached;
namespace this_coro = asio::this_coro;

#define LEAN_AND_MEAN
//#define NOMINMAX
#include <Windows.h>


#include <Semaphore.h>



//
//void chargeTimer(HANDLE timer, long long time_ms) {
//	constexpr auto ms = 10000LL;
//	LARGE_INTEGER time{ .QuadPart = -time_ms * ms };
//	if (!SetWaitableTimer(timer, &time, 0, NULL, NULL, 0)) {
//		std::cout << "set timer error\n";
//		std::exit(-1);
//	}
//}
//
//HANDLE createTimer(long long time_ms) {
//	const auto timer = CreateWaitableTimer(NULL, FALSE, NULL);
//	if (timer == NULL) {
//		std::cout << "timer error\n";
//		std::exit(-1);
//	}
//	chargeTimer(timer, time_ms);
//	return timer;
//}

//VOID CALLBACK TimerAPCProc(LPVOID, DWORD, DWORD);
//void chargeTimerEx(HANDLE timer, long long time_ms);

//struct Timer {
//private:
//	HANDLE handle;
//	LARGE_INTEGER time{ .QuadPart = 0 };
//
//	using callback = std::function<void(Timer*)>;
//	callback cb;
//	static VOID CALLBACK cb_impl(LPVOID lpArg, DWORD, DWORD) {
//		const auto _this = (Timer*)lpArg;
//		if (_this->cb)
//			_this->cb(_this);
//		//_this->relaunch();
//	}
//public:
//	Timer() {
//		handle = CreateWaitableTimerEx(NULL, NULL, NULL, TIMER_ALL_ACCESS);
//		if (handle == NULL) {
//			std::cout << "timer error\n";
//			std::exit(1);
//		}
//	}
//	Timer(callback cb) : Timer() {
//		setCallback(cb);
//	}
//	Timer(callback cb, long long time_ms) : Timer(cb) {
//		launch(time_ms);
//	}
//	void setCallback(callback cb) {
//		this->cb = cb;
//	}
//	void relaunch() {
//		if (!SetWaitableTimerEx(handle, &time, 0, &cb_impl, this, NULL, NULL)) {
//			std::cout << "set timer error\n";
//			std::exit(2);
//		}
//	}
//	void launch(long long time_ms) {
//		time = LARGE_INTEGER{
//			.QuadPart = -10000LL * time_ms
//		};
//		relaunch();
//	}
//};


//
//void setTimeout(long long time_ms, VOID(*cb)(LPVOID, DWORD, DWORD)) {
//	const auto handle = CreateWaitableTimerEx(NULL, NULL, NULL, TIMER_ALL_ACCESS);
//	if (handle == NULL) {
//		std::cout << "timer error\n";
//		std::exit(1);
//	}
//	const auto time = LARGE_INTEGER{
//		.QuadPart = -10000LL * time_ms
//	};
//	if (!SetWaitableTimerEx(handle, &time, 0, cb, NULL, NULL, NULL)) {
//		std::cout << "set timer error\n";
//		std::exit(2);
//	}
//	CloseHandle(handle);
//}




Semaphore sem   aphore(4, 4);

int missed = 0, served = 0, serving = 0;

void print() {
	std::cout << std::setw(10) << "missed" << std::setw(10) << "served" << std::setw(10) << "serving" << '\n';
	std::cout << std::setw(10) << missed << std::setw(10) << served << std::setw(10) << serving << '\n';
}

//VOID CALLBACK deleteCar(LPVOID, DWORD, DWORD)
//{
//	semaphore.release();
//	++served;
//	--serving;
//	print();
//}
//VOID CALLBACK newCar(LPVOID, DWORD, DWORD)
//{
//	if (not semaphore.acquire())
//		++missed;
//	else {
//		++serving;
//		setTimeout(fillingTime, deleteCar);
//	}
//	print();
//	setTimeout(arrivalTime, newCar);
//}


constexpr auto arrivalTime = 3'000;
constexpr auto fillingTime = 15'000;

asio::awaitable<void> fill_car() {
	try {
		++serving;
		asio::deadline_timer filling_timer(co_await this_coro::executor, boost::posix_time::milliseconds(fillingTime));
		co_await filling_timer.async_wait(use_awaitable);
		semaphore.release();
		--serving;
		++served;
		std::cout << std::setw(15) << "\ncar filled\n";
		print();
	}
	catch (const std::exception& ex) {
		std::cout << "fill car error: " << ex.what() << '\n';
		co_return;
	}
}


asio::awaitable<void> car_spawner() {
	try {
		auto executor = co_await this_coro::executor;
		for (;;) {
			asio::deadline_timer car_timer(executor, boost::posix_time::milliseconds(arrivalTime));
			co_await car_timer.async_wait(use_awaitable);

			if (semaphore.acquire()) 
				asio::co_spawn(executor, fill_car(), detached);
			else 
				++missed;
			std::cout << std::setw(15) << "\ncar arrived\n";
			print();
		}
	}
	catch (const std::exception& ex) {
		std::cout << "car spawner error: " << ex.what() << '\n';
		co_return;
	}
}


int main()
{
	asio::io_context ctx{ 1 };
	asio::co_spawn(ctx, car_spawner(), detached);
	ctx.run();




}

//std::system("chcp 1251 && cls");
////std::cout << std::this_thread::get_id() << '\n';

//setTimeout(arrivalTime, newCar);

//while (1)
//	SleepEx(INFINITE, TRUE);

//std::vector<HANDLE> handles{
//	timer100, timer450, timer500, timer1200
//};

//while (1) {
//	const auto status = WaitForMultipleObjects(handles.size(), handles.data(), FALSE, INFINITE);

//	if (status == WAIT_FAILED) {
//		std::cout << "wait error\n";
//		std::exit(-1);
//	}
//	if (status == WAIT_TIMEOUT) {
//		std::cout << "timeout error\n";
//		std::exit(-1);
//	}
//	
//	// WAIT_OBJECT_0 to (WAIT_OBJECT_0 + nCount– 1)
//	const auto waited = status - WAIT_OBJECT_0;
//	if (waited >= 0 and waited < handles.size()) {
//		const auto handle = handles[waited];

//		//CancelWaitableTimer(handle);
//		if (handle == timer100) {
//			std::cout << "100\n";
//			chargeTimer(handle, 100);
//		}
//		else if (handle == timer450) 
//		{
//			std::cout << "450\n";
//			chargeTimer(handle, 450);
//		}
//		else if (handle == timer500) 
//		{
//			std::cout << "500\n";
//			chargeTimer(handle, 500);
//		}
//		else if (handle == timer1200) 
//		{
//			std::cout << "1200\n";
//			chargeTimer(handle, 1200);
//		}
//		continue;
//	}
//	// WAIT_ABANDONED_0 to (WAIT_ABANDONED_0 + nCount– 1)
//	//const auto abandoned = status - WAIT_ABANDONED_0;
//	//if (waited >= 0 and waited < handles.size()) {
//	//	const auto handle = handles[waited];
//	//	if (handle == timer450)
//	//		std::cout << "450 abandoned\n";
//	//	else if (handle == timer500)
//	//		std::cout << "500 abandoned\n";
//	//	else if (handle == timer1200)
//	//		std::cout << "1200 abandoned\n";
//	//	continue;
//	//}
//}


//while (1) {
//	int input;
//	std::cin >> input;	
//	switch (input) {
//	case 1:
//		std::cout << WaitForSingleObject(semaphor, 0) << '\n';
//		break;
//	case -1:
//		std::cout << ReleaseSemaphore(semaphor, 1, NULL) << '\n';
//		break;
//	default: continue;
//	}
//}