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

constexpr auto MAX_SERVER_COUNT = 50UL;

struct ServerSlots{
	struct ClientSlot {
		std::string event_name;
		sf::RectangleShape rect;
	};
private:
	struct Slot : ClientSlot {
		std::binary_semaphore constructed = std::binary_semaphore{ 1 };
		std::binary_semaphore acquired = std::binary_semaphore{ 1 };
	};
	std::array<Slot, MAX_SERVER_COUNT> slots;
public:
	Slot& getServerSlot() {
		const auto slot_iter = std::find_if(slots.begin(), slots.end(), [](Slot& slot) {
			return slot.constructed.try_acquire();
			}
		);
		assert(slot_iter != slots.end());
		auto& slot = *slot_iter;

		slot.event_name = std::format("SlotSemaphore{}", std::distance(slots.begin(), slot_iter));

		return slot;
	}
	ClientSlot& getClientSlot() {
		const auto slot_iter = std::find_if(slots.begin(), slots.end(), [](Slot& slot) {
			return slot.acquired.try_acquire();
			}
		);
		assert(slot_iter != slots.end());
		auto& slot = *slot_iter;

		return static_cast<ClientSlot&>(slot);
	}
};


void* SharedState() {
	const auto _filemapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof ServerSlots, L"BattleshipSharedState");
	const auto code = GetLastError();
	std::cout << (code == ERROR_ALREADY_EXISTS) << '\n';
	if (not _filemapping) {
		std::cout << "mapping error: " << code;
		std::exit(1);
	}
	else
		std::cout << "created mapping\n";
	const auto _shared = MapViewOfFile(_filemapping, FILE_MAP_ALL_ACCESS, 0, 0, sizeof ServerSlots);
	if (not _shared) {
		std::cout << "view error: " << GetLastError() << '\n';
		std::exit(2);
	}
	const auto s_ptr = reinterpret_cast<ServerSlots*>(_shared);

	if (code == ERROR_ALREADY_EXISTS) {
		std::construct_at(s_ptr);

	}
	return s_ptr;
}


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
