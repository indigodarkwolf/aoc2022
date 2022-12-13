#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <map>
#include <functional>

std::vector<std::string> split(std::string s, char delimiter)
{
	std::vector<std::string> strings;
    std::istringstream f(s);
    std::string l;    
    while (std::getline(f, l, delimiter)) {
        strings.push_back(l);
	}	
	return strings;
}

int main()
{
	std::ifstream infile("input.txt", std::ios_base::in);
	if (!infile.is_open()) {
		return -1;
	}

	std::string line;

	int x = 1;
	int clock_cycle = 1;
	int checkpoint = 20;

	int sum_of_strengths = 0;

	std::stringstream display;

	auto cycle = [&](int cycles){
		for(int c = 0; c < cycles; ++c) {
			const int screen_x = (clock_cycle-1) % 40;
			display << ((std::abs(screen_x - x) <= 1) ? '#' : '.');

			++clock_cycle;

			if(clock_cycle > checkpoint) {
				const int strength = checkpoint * x;
				// std::cout << "Cycle " << checkpoint << " strength: " << strength << std::endl;
				sum_of_strengths += strength;
				checkpoint += 40;
			}
		}
	};

	while (std::getline(infile, line)) {
		std::cout << line << std::endl;
		if(line.length() > 0) {
			const uint32_t op = *reinterpret_cast<const uint32_t *>(line.c_str());
			switch(op) {
				case 'poon':
					cycle(1);
					break;
				case 'xdda':
					cycle(2);
					x += strtol(line.c_str() + 5, nullptr, 10);
					break;
				default:
					std::cout << "->" << *reinterpret_cast<const uint32_t *>(line.c_str()) << std::endl;
					break;
			}
		}
	}

	if(clock_cycle <= 240) {
		cycle(240 - clock_cycle);
	}

	std::cout << "What is the sum of these six signal strengths?" << std::endl;
	std::cout << "Part 1) " << sum_of_strengths << std::endl;

	std::cout << "What eight capital letters appear on your CRT?" << std::endl;
	std::cout << "Part 2) " << std::endl;

	std::string display_str = display.str();
	for(int i=0; i<240; i += 40) {
		std::cout << display_str.substr(i, 40) << std::endl;
	}
	return 0;
}