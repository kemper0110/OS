#include <iostream>
#include <random>
#include <array>




#include "SharedBattleship.h"
#include "PipeBattleship.h"

int main()
{
	std::unique_ptr<Battleship> battleship = std::make_unique<PipeBattleship>();

	battleship->play();



	std::cout << "game ended\n";
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

