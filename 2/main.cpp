#include <iostream>
#include <fstream>
#include <string>
#include <queue>

std::priority_queue<int> calories_set;

// Play values
// Rock = 1
// Paper = 2
// Scissors = 3

// Result values
// Lose = 0
// Draw = 3
// Win = 6

constexpr int rps_result[3][3] = {
	{1+3, 2+6, 3+0},
	{1+0, 2+3, 3+6},
	{1+6, 2+0, 3+3}
};

constexpr int ldw_result[3][3] = {
	{3+0, 1+3, 2+6},
	{1+0, 2+3, 3+6},
	{2+0, 3+3, 1+6}
};

int main()
{
	std::ifstream infile("input.txt", std::ios_base::in);
	if (!infile.is_open()) {
		return -1;
	}

	std::string line;

	int rps_score = 0;
	int ldw_score = 0;
	while (std::getline(infile, line)) {
		std::cout << line << std::endl;
		if(line.length() >= 3) {
			rps_score += rps_result[line[0]-'A'][line[2]-'X'];
			ldw_score += ldw_result[line[0]-'A'][line[2]-'X'];
		}
	}

	std::cout << "RPS Score: " << rps_score << std::endl;
	std::cout << "LDW Score: " << ldw_score << std::endl;
}