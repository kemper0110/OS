#pragma once

#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows
#include <Windows.h>


struct Semaphore {
	HANDLE handle;
	Semaphore(int init, int max, std::wstring_view name) {
		handle = CreateSemaphore(NULL, init, max, name.data());
	}
	Semaphore(int init, int max) {
		handle = CreateSemaphore(NULL, init, max, NULL);
	}
	bool acquire() {
		const auto status = WaitForSingleObject(handle, INFINITE);
		switch (status) {
		case WAIT_TIMEOUT:
			throw std::runtime_error("infinite timeout");
		case WAIT_OBJECT_0:
			return true;
		case WAIT_ABANDONED_0:
			throw std::runtime_error("abandoned");
		case WAIT_FAILED:
			throw std::runtime_error("wait failed");
		default: throw status;
		}
	}
	bool try_acquire() {
		const auto status = WaitForSingleObject(handle, NULL);
		switch (status) {
		case WAIT_TIMEOUT:
			return false;
		case WAIT_OBJECT_0:
			return true;
		case WAIT_ABANDONED_0:
			throw std::runtime_error("abandoned");
			break;
		case WAIT_FAILED:
			throw std::runtime_error("wait failed");
			break;
		default: throw status;
		}
	}
	LONG release(int count = 1) {
		LONG prev_count;
		if (!ReleaseSemaphore(handle, count, &prev_count))
			throw std::runtime_error("release error");
		return prev_count;
	}
};