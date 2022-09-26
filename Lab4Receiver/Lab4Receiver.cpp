#define LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <array>
#include <random>
#include <iomanip>

template<class Lambda>
struct defer : Lambda {
	~defer() { Lambda::operator()(); }
};


auto getGenerator(int from, int to) {
	return [gen = std::random_device(), distr = std::uniform_int_distribution(from, to)]() mutable {
		return distr(gen);
	};
}

void printGeometry() {
	constexpr auto figures = std::array{
	"rectangle",
	"triangle",
	"cylinder",
	"sphere",
	"ellipse",
	"tetraedr",
	"TOR"
	};
	const auto colors = std::array{
		"red", "black", "white", "yellow",
		"green", "purple", "pink", "blue",
		"orange", "gray", "brown", "deep blue",
	};

	auto figure_generator = getGenerator(0, figures.size() - 1);
	auto color_generator = getGenerator(0, colors.size() - 1);

	for (int i = 0; i < 100; ++i) {
		std::cout << std::setw(10) << colors[color_generator()] << std::setw(15) << figures[figure_generator()] << '\n';
	}
}


int main()
{
	std::system("chcp 65001 && cls");

	const auto event = CreateEvent(NULL, TRUE, FALSE, L"Lab4Signal");

	if (event == NULL) {
		std::cout << "Event error: " << GetLastError();
		std::exit(-1);
	}
	defer d{ [event] { CloseHandle(event); } };


	std::cout << "waiting event\n";
	const auto wait_status = WaitForSingleObject(event, INFINITE);

	if (wait_status == WAIT_OBJECT_0) 
		printGeometry();
	else
		std::cout << "error\n";
}

