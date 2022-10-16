#include <iostream>
#include <SFML/Graphics.hpp>
#define NOMINMAX
#define LEAN_AND_MEAN
#include <Windows.h>


#include <Semaphore.h>

constexpr auto MAX_SERVER_COUNT = 50UL;


int main()
{
	auto semaphore = Semaphore(0, MAX_SERVER_COUNT, L"DrawSemaphore");

	semaphore.acquire();

	while (true) {
		std::cout << "press any key to send random rectangle\n";
		std::cout << "press q to quit\n";

		const auto input = std::cin.get();
		if (input == 'q') {
			break;
		}
		sf::RectangleShape rect({ 100.f, 100.f });
		rect.setPosition(rand(), rand());
		rect.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));

		// send rect
	}
	std::cout << "quiting\n";
	semaphore.release();
}
