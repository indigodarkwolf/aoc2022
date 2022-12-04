#include <iostream>
#include <fstream>
#include <string>
#include <queue>

std::priority_queue<int> calories_set;

int main()
{
	std::ifstream infile("input.txt", std::ios_base::in);
	if (!infile.is_open()) {
		return -1;
	}

	std::string line;

	int calories = 0;
	int elf = 1;
	while (std::getline(infile, line)) {
		std::cout << line << std::endl;
		if(line.length() == 0) {
			std::cout << elf << ": " << calories << std::endl;
			calories_set.push(calories);

			calories = 0;
			++elf;
		}

		calories += atoi(line.c_str());
	}

	std::cout << "Most calories: " << calories_set.top() << std::endl;

	int total = 0;
	for(int i=0; i < 3; ++i) {
		if(calories_set.empty()) {
			break;
		}
		total += calories_set.top();
		calories_set.pop();
	}
	std::cout << "Top 3 total: " << total << std::endl;

	return 0;
}