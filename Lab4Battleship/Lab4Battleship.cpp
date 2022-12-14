#include <iostream>
#include <random>
#include <array>




#include "SharedBattleship.h"
#include "PipeBattleship.h"

int main()
{
	std::system("chcp 1251 && cls");
	std::unique_ptr<Battleship> battleship = std::make_unique<SharedBattleship>();
	//std::unique_ptr<Battleship> battleship = std::make_unique<PipeBattleship>();

	battleship->play();

	std::cout << "game ended\n";
	if (battleship->isWon())
		std::cout << "We won\n";
	else
		std::cout << "We losed\n";
	if (battleship->isDefeated())
		std::cout << "We have been defeated\n";
	else
		std::cout << "They have been defeated\n";

	/*
		--	First player
		wait until second player enters
			             <--------------------------------------|
		check if I have been defeated	->	tell about it		|
		make move												|
		wait until second player makes move						|
				      ------------------------------------------|

		--  Second player
		connect
		I'm second so
		<--------------|
		wait move      |
		make move	   |
		---------------|
	*/
}

