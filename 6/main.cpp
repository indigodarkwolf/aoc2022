#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <stack>

bool is_unique(const char *pattern, int pattern_len, int *buckets)
{
	for(size_t i=0; i<pattern_len; ++i) {
		if(buckets[pattern[i]-'a'] > 1) {
			return false;
		} 
	}

	return true;
}

int find_marker(const char *data, size_t data_len, int marker_len)
{
	int buckets[26];
	for(int i=0; i<sizeof(buckets)/sizeof(int); ++i) {
		buckets[i] = 0;
	}

	for(size_t i=0; i<marker_len; ++i) {
		buckets[data[i]-'a']++;
	}

	for(size_t i=marker_len; i<data_len; ++i) {
		if(is_unique(data + i - marker_len, marker_len, buckets)) {
			return i;
		}
		buckets[data[i-marker_len]-'a']--;
		buckets[data[i]-'a']++;
	}

	return -1;
}

int main()
{
	std::ifstream infile("input.txt", std::ios_base::in);
	if (!infile.is_open()) {
		return -1;
	}

	std::string line;

	while (std::getline(infile, line)) {
		std::cout << line << std::endl;
		if(line.length() > 14) {
			size_t len = line.length();
			const char *s = line.c_str();

			std::cout << "first (packet) marker after character " << find_marker(s, len, 4) << std::endl;
			std::cout << "first (message) marker after character " << find_marker(s, len, 14) << std::endl;
		}
	}

	return 0;
}