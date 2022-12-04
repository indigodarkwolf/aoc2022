#include <iostream>
#include <fstream>
#include <string>
#include <queue>

static int get_priority(uint64_t common_types)
{
	int priority = 0;
	while(common_types != 0) {
		++priority;
		common_types >>= 1;
	}
	// std::cout << "Common item: " << (char)(priority + 'A' - 1) << std::endl;

	if(priority > 26) {
		priority -= 26 + 6; // These characters don't count: [\]^_`
	} else{
		priority += 26;
	}
	// std::cout << "Priority: " << (int)priority << std::endl;
	return priority;
}

int main()
{
	std::ifstream infile("input.txt", std::ios_base::in);
	if (!infile.is_open()) {
		return -1;
	}

	std::string line;

	int sum = 0;

	uint64_t set_types = ~0ULL;
	int set_count = 0;
	int set_sum = 0;

	while (std::getline(infile, line)) {
		std::cout << line << std::endl;

		if(line.length() > 0) {
			int len = line.length();
			int left = len >> 1;

			uint64_t left_types = 0;
			uint64_t right_types = 0;

			for(int i=0; i<left; ++i) {
				left_types |= 1ULL << (line[i] - 'A');
			}
			for(int i=left; i<len; ++i) {
				right_types |= 1ULL << (line[i] - 'A');
			}
			uint64_t common_types = left_types & right_types;
			sum += get_priority(common_types);

			set_types &= (left_types | right_types);
			++set_count;
			if(set_count >= 3) {
				set_sum += get_priority(set_types);
				set_types = ~0ULL;
				set_count = 0;
			}
		}
	}

	std::cout << "Sum: " << sum << std::endl;
	std::cout << "Set sum: " << set_sum << std::endl;
	return 0;
}