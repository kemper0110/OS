#pragma once

#include <queue>
#include <random>
#include <array>
#include <numeric>


class Battleship
{	
protected:
	struct Message {
		enum Reason : short int {
			Null, Fired, Beaten
		};
		Reason reason;
		int target;
	};
	Message makeRandomMove();
	void print();

	virtual bool waitBeginning() = 0;
	virtual void send(const Message& message) = 0;
	virtual Message receive() = 0;
public:

	Battleship();
	virtual ~Battleship();
	bool isDefeated();
	bool isWon();
	void play();
protected:
	static constexpr unsigned long long SHIP_COUNT = 10;
	static constexpr unsigned long long FIELD_SIZE = 25;

	enum Cell : char {
		Empty = ' ', Ship = 'T', FiredEmpty = 'O', FiredShip = 'X'
	};
	int player;
	bool ended = false;
	std::array<Cell, FIELD_SIZE> field;
	std::queue<size_t> strategy;
	//std::array<Cell, FIELD_SIZE> EnemyField;

	int defeatedCount = 0;
	std::random_device rnd;
	std::uniform_int_distribution<> distr = std::uniform_int_distribution<int>(0, std::size(field) - 1);
};




