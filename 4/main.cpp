#include <iostream>
#include <fstream>
#include <string>
#include <queue>

int num_contained(const int *r0, const int *r1)
{
	if(r1[0] < r0[0]) {
		return num_contained(r1, r0);
	} else if(r0[0] == r1[0]) {
		return 1 + std::min(r0[1], r1[1]) - r0[0];
	}

	return std::max(1 + r0[1] - r1[0], 0);
}

int num_zones(const int *r)
{
	return 1 + r[1] - r[0];
}

bool wholly_contained2(const int *r0, const int *r1)
{
	return num_contained(r0, r1) >= std::min(num_zones(r0), num_zones(r1));
}

bool wholly_contained(const int *r0, const int *r1)
{
	if(r1[0] < r0[0]) {
		return wholly_contained(r1, r0);
	} else if(r0[0] == r1[0]) {
		return true;
	}

	return (r1[1] <= r0[1]);
}

int main()
{
	std::ifstream infile("input.txt", std::ios_base::in);
	if (!infile.is_open()) {
		return -1;
	}

	std::string line;

	int num_contained1 = 0;
	int num_contained2 = 0;
	int num_overlap = 0;

	int count = 0;
	int ranges[4] = {0, 0, 0, 0};

	while (std::getline(infile, line)) {
		std::cout << line << std::endl;

		if(line.length() > 0) {
			for(char c : line) {
				if(isdigit(c)) {
					ranges[count] *= 10;
					ranges[count] += c - '0';
				} else {
					++count;
				}
			}

			const bool contained1 = wholly_contained(ranges, ranges+2);
			const bool contained2 = wholly_contained2(ranges, ranges+2);
			const bool overlap = num_contained(ranges, ranges+2) > 0;
			std::cout << ranges[0] << "-" << ranges[1] << "," << ranges[2] << "-" << ranges[3] << (contained1 ? "*" : "") << (contained2 ? "+" : "") << std::endl;

			if(contained1) {
				++num_contained1;
			}
			if(contained2) {
				++num_contained2;
			}
			if(overlap) {
				++num_overlap;
			}
			for(int i=0; i<4; ++i) {
				ranges[i] = 0;
			}
			count = 0;
		}
	}

	std::cout << "Num contained: " << num_contained1 << std::endl;
	std::cout << "Num contained2: " << num_contained2 << std::endl;
	std::cout << "Num overlapping: " << num_overlap << std::endl;
	return 0;
}