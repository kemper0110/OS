#include "Battleship.h"
#include <cassert>
#include <iostream>

#define LEAN_AND_MEAN
#include <Windows.h>


bool Battleship::isDefeated()
{
	return std::count(field.begin(), field.end(), Cell::FiredShip) == SHIP_COUNT;
}

bool Battleship::isWon()
{
	return defeatedCount == SHIP_COUNT;
}

void Battleship::play()
{
	std::cout << "wating\n";
	const auto isFirst = waitBeginning();
	std::cout << (isFirst ? "first" : "second") << " player\n";

	const auto fire = [this] {
		while (true) {
			const auto move = makeRandomMove();
			Sleep(100);
			send(move);
			Sleep(100);
			if (isDefeated() or isWon()) return 1;
			auto response = receive();
			print();
			std::cout << "firing\n";
			Sleep(2000);
			if (response.reason == response.Beaten) {
				defeatedCount++;
				std::cout << "I hit\n";
			}
			else {
				std::cout << "I missed\n";
				return 0;
			}
			if (isDefeated() or isWon()) return 1;
		}
	};

	const auto defense = [this] {
		while (true) {
			Sleep(100);
			if (isDefeated() or isWon()) return 1;
			auto move = receive();
			print();
			std::cout << "defensing\n";
			Sleep(2000);
			assert(move.target < FIELD_SIZE&& move.target >= 0);
			if (field[move.target] == Ship) {
				//std::cout << "He Hit\n";
				field[move.target] = FiredShip;
				move.reason = move.Beaten;
				send(move);
				if (isDefeated() or isWon()) return 1;
			}
			else {
				//std::cout << "He Missed\n";
				if (field[move.target] == Empty)
					field[move.target] = FiredEmpty;
				send(move);
				return 0;
			}
		}
	};


	while (true) {
		if (isFirst) {
			if (isDefeated() or isWon()) break;
			if (fire()) break;
			if (isDefeated() or isWon()) break;
			if (defense()) break;
		}
		else {
			if (isDefeated() or isWon()) break;
			if (defense()) break;
			if (isDefeated() or isWon()) break;
			if (fire()) break;
		}
	}
}

Battleship::Battleship()
{
	for (auto i = 0UL; i < SHIP_COUNT; ++i)
		field[distr(rnd)] = Cell::Ship;

	std::deque<size_t> indices(FIELD_SIZE);
	std::iota(indices.begin(), indices.end(), 0);
	std::shuffle(indices.begin(), indices.end(), rnd);
	strategy = decltype(strategy)(std::move(indices));
}

Battleship::Message Battleship::makeRandomMove()
{
	Message m{
		.reason = Message::Fired,
		.target = (int)strategy.front()
	};
	strategy.pop();
	return m;
}

void Battleship::print()
{
	//std::system("cls");
	for (int row = 0; row < 5; ++row) {
		for (int col = 0; col < 5; ++col)
			std::cout << field[row * 5 + col];
		std::cout << '\n';
	}
	std::cout << "defeated: " << defeatedCount << '\n';
}

Battleship::~Battleship()
{
}
