#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <stack>

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
	std::vector<std::list<char>> piles;
	std::vector<std::list<char>> ordered_piles;

	int phase = 0;
	while (std::getline(infile, line)) {
		std::cout << line << std::endl;

		if(line.length() > 0) {
			switch(phase) {
				case 0:
					piles.resize((line.length() + 1) >> 2);
					ordered_piles.resize((line.length() + 1) >> 2);
					++phase;
					[[fallthrough]]
				case 1: {
					for(int i=1; i<line.length(); i+=4) {
						if(isdigit(line[i])) {
							++phase;

							for(int i=0; i<piles.size(); ++i) {
								std::cout << i << ": ";
								for(char c : piles[i]) {
									std::cout << "[" << c << "]";
								}
								std::cout << std::endl;
							}
							break;
						} else if(line[i] != ' ') {
							piles[i >> 2].push_back(line[i]);
							ordered_piles[i >> 2].push_back(line[i]);
						}
					}
				} break;
				case 2: {
					auto tokens = split(line, ' ');
					std::cout << "Tokens: ";
					for(auto &t : tokens) {
						std::cout << "\"" << t << "\", ";
					}
					std::cout << std::endl;
					int qty = strtol(tokens[1].c_str(), nullptr, 10);
					int from = strtol(tokens[3].c_str(), nullptr, 10);
					int to = strtol(tokens[5].c_str(), nullptr, 10);
					std::cout << "Values: " << qty << ", " << from << ", " << to << std::endl;

					for(int i=0; i<qty; ++i) {
						char c = piles[from-1].front();
						piles[from-1].pop_front();
						piles[to-1].push_front(c);
					}

					std::stack<char> stash;

					for(int i=0; i<qty; ++i) {
						char c = ordered_piles[from-1].front();
						ordered_piles[from-1].pop_front();
						stash.push(c);
					}

					for(int i=0; i<qty; ++i) {
						char c = stash.top();
						stash.pop();
						ordered_piles[to-1].push_front(c);
					}

				} break;
			}
		}
	}

	std::cout << "Tops: ";
	for(auto &pile : piles) {
		if(pile.empty()) {
			std::cout << " ";
		} else {
			std::cout << pile.front();
		}
	}
	std::cout << std::endl;

	std::cout << "Tops2: ";
	for(auto &pile : ordered_piles) {
		if(pile.empty()) {
			std::cout << " ";
		} else {
			std::cout << pile.front();
		}
	}
	std::cout << std::endl;

	return 0;
}