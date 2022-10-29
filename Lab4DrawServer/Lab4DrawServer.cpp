#include <format>
#include <cassert>
#include <iostream>
#include <semaphore>
#include <mutex>
#include <array>
#include <vector>
#include <SFML/Graphics.hpp>
#define NOMINMAX
#define LEAN_AND_MEAN
#include <Windows.h>


#include <Semaphore.h>

#include <Lab4DrawLibrary/Lab4DrawLibrary.h>




int main()
{
	sf::ContextSettings settings(0U, 0U, 8U);
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Draw", sf::Style::Default, settings);
	window.setFramerateLimit(60);

	std::vector<Rect> rectangles;


	auto& slot = ServerSlots::getServerSlots()->getServerSlot();
	const auto event = CreateEventA(NULL, NULL, FALSE, slot.event_name);
	if (event == NULL) {
		std::cout << "event not created: " << GetLastError() << '\n';
		std::exit(1);
	}

	//const auto semaphore = CreateSemaphore(NULL, 0, MAX_SERVER_COUNT, NULL);
	//ReleaseSemaphore(semaphore, 1, NULL);
	auto semaphore = Semaphore(0, MAX_SERVER_COUNT, L"DrawSemaphore");
	semaphore.release();


	while (window.isOpen()) {
		{
			sf::Event event;
			while (window.pollEvent(event)) {
				switch (event.type) {
				case sf::Event::Closed:
					std::cout << "waiting client disconnection\n";
					semaphore.try_acquire();
					slot.working.release();
					std::cout << "closed\n";
					window.close();
					break;
				}
			}
		}

		const auto wait_status = WaitForSingleObject(event, NULL);
		switch (wait_status) {
		case WAIT_OBJECT_0:
			rectangles.push_back(slot.rect);
			break;
		case WAIT_TIMEOUT:
			break;
		case WAIT_FAILED:
		case WAIT_ABANDONED:
			std::cout << "wait failed: " << GetLastError();
			std::exit(1);
		}

		window.clear(sf::Color::White);

		for (auto const& rect : rectangles) 
			window.draw(rect.data(), rect.size(), sf::Quads);

		window.display();
	}


}
