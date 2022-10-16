#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#define NOMINMAX
#define LEAN_AND_MEAN
#include <Windows.h>


#include <Semaphore.h>

constexpr auto MAX_SERVER_COUNT = 50UL;

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Draw");

	std::vector<sf::RectangleShape> rectangles;


	//const auto semaphore = CreateSemaphore(NULL, 0, MAX_SERVER_COUNT, NULL);
	//ReleaseSemaphore(semaphore, 1, NULL);
	auto semaphore = Semaphore(0, MAX_SERVER_COUNT, L"DrawSemaphore");
	semaphore.release();


	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				semaphore.acquire();
				window.close();
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code) {
				case sf::Keyboard::Space:
					rectangles.push_back(sf::RectangleShape({10.f, 10.f}));
					rectangles.back().setFillColor(sf::Color::Red);
					//rectangles.back().setPosition()
					break;
				}
				break;
			}
		}

		window.clear();

		for (auto const& rect : rectangles)
			window.draw(rect);

		window.display();
	}


}
