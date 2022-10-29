#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>
#define NOMINMAX
#define LEAN_AND_MEAN
#include <Windows.h>


#include <Semaphore.h>

#include <Lab4DrawLibrary/Lab4DrawLibrary.h>



int main()
{
	std::cout << "client started and waiting\n";
	auto semaphore = Semaphore(0, MAX_SERVER_COUNT, L"DrawSemaphore");
	semaphore.acquire();
	std::cout << "client connected\n";

	auto& slot = ServerSlots::getServerSlots()->getClientSlot();
	const auto event = CreateEventA(NULL, NULL, FALSE, slot.event_name);
	if (event == NULL) {
		std::cout << "Event error: " << GetLastError() << '\n';
		std::exit(1);
	}
	const auto colors = std::array{
		sf::Color::Blue, sf::Color::Green, sf::Color::Red, 
		sf::Color::Magenta, sf::Color::Yellow, sf::Color(0x2f, 0x2f, 0x2f), 
		sf::Color(0x51, 0x2f, 0xa1), sf::Color(255, 69, 0),
		sf::Color(0, 0, 255), sf::Color(0, 197, 205)
	};
	const auto sizes = std::array{
		sf::Vector2f{ 200.f, 200.f },
		sf::Vector2f{ 100.f, 100.f },
		sf::Vector2f{ 200.f, 140.f },
		sf::Vector2f{ 140.f, 200.f },
		sf::Vector2f{ 300.f, 200.f }
	};
	std::random_device rnd;
	// 1280, 720

	std::uniform_real_distribution<float> 
		width_distr(WINDOW_WIDTH * 10 / 100, WINDOW_WIDTH * 90 / 100),
		height_distr(WINDOW_HEIGHT * 10 / 100, WINDOW_HEIGHT * 90 / 100);


	while (true) {
		std::cout << "press any key to send random rectangle\n";
		std::cout << "press q to quit\n";

		const auto input = std::cin.get();
		if (input == 'q') {
			break;
		}

		std::array<sf::Color, 4> rect_colors;
		std::sample(colors.begin(), colors.end(), rect_colors.begin(), rect_colors.size(), rnd);

		sf::Vector2f size;
		std::sample(sizes.begin(), sizes.end(), &size, 1, rnd);
		sf::Vector2f pos = {width_distr(rnd), height_distr(rnd)};

		const auto vertices = std::array{
			sf::Vertex{pos, rect_colors[0]},
			sf::Vertex{pos + sf::Vector2f{ size.x, 0.f }, rect_colors[1]},
			sf::Vertex{pos + size, rect_colors[2]},
			sf::Vertex{pos + sf::Vector2f{ 0.f, size.y }, rect_colors[3]},
		};

		slot.rect = vertices;
		SetEvent(event);

		std::cout << "Event sended\n";
	}
	std::cout << "quiting\n";
	semaphore.release();
	slot.acquired.release();
}
