#pragma once
#include "Battleship.h"
#define LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include "perror.h"
#include <mutex>
#include <atomic>
#include <semaphore>

class SharedBattleship :
	public Battleship
{
	//HANDLE event;
	struct SharedState {
		Message message{};
		std::atomic_bool startup = false;

		//std::atomic_bool must_read;
		std::mutex message_mtx;
		std::binary_semaphore pending[2] = {
			std::binary_semaphore{ 1 },
			std::binary_semaphore{ 1 }
		};
		//std::atomic_bool pending[2];

		//std::binary_semaphore pending[2] = {
		//	std::binary_semaphore{ 1 },
		//	std::binary_semaphore{ 0 },
		//};
		//std::atomic_bool sended;
		//std::atomic_bool received;

		//std::binary_semaphore sended[2] = {
		//	std::binary_semaphore{ 1 },
		//	std::binary_semaphore{ 1 }
		//};
		//std::binary_semaphore received[2] = {
		//	std::binary_semaphore{ 1 },
		//	std::binary_semaphore{ 1 }
		//};
		
		//std::binary_semaphore sending = std::binary_semaphore{ 1 };
		//std::binary_semaphore receiving = std::binary_semaphore{ 1 };
		//std::binary_semaphore sended[2] = {
		//    std::binary_semaphore{1},
		//    std::binary_semaphore{1}
		//};
		//std::binary_semaphore received[2] = {
		//    std::binary_semaphore{1},
		//    std::binary_semaphore{1}
		//};
		//std::atomic_bool pending;
		//std::atomic_bool receiving;

		//std::mutex sending;
		//std::mutex receiving;
	};
	SharedState* shared = nullptr;
	HANDLE sendedEvent, receivedEvent;
public:
	SharedBattleship();
	~SharedBattleship();
	// Унаследовано через Battleship
	virtual bool waitBeginning() override;
	virtual void send(const Message& message) override;
	virtual Message receive() override;
private:
	LPVOID _shared;
	HANDLE _filemapping;
};

