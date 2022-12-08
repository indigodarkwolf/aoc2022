#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <map>
#include <functional>

#include "gif.h"

int main()
{
	std::ifstream infile("input.txt", std::ios_base::in);
	if (!infile.is_open()) {
		return -1;
	}

	std::string line;

	std::vector<std::string> forest;

	while (std::getline(infile, line)) {
		std::cout << line << std::endl;
		if(line.length() > 0) {
			forest.push_back(line);
		}
	}

	const int w = forest[0].length();
	const int h = forest.size();

	{
		int32_t *img = new int32_t[h * w];

		int i=0;
		for(int y=0; y<h; ++y) {
			for(int x=0; x<w; ++x) {
				int c = forest[y][x]-'0';
				c = c * 255 / 9;
				c |= c << 8;
				c |= c << 16;
				img[i] = c | 0xff000000; // byte order is ARGB
				++i;
			}
		}

		GifWriter writer;
		if(GifBegin(&writer, "input.gif", w, h, 1, 8, false)) {
			GifWriteFrame(&writer, reinterpret_cast<uint8_t *>(img), w, h, 1, 8, false);
			GifEnd(&writer);
		}

		delete [] img;
	}

	std::vector<std::vector<int8_t>> visibility(h, std::vector<int8_t>(w, 0));

	constexpr const char no_tree = '0'-1;

	constexpr const int8_t vis_left = (1 << 0);
	constexpr const int8_t vis_right = (1 << 1);
	constexpr const int8_t vis_up = (1 << 2);
	constexpr const int8_t vis_down = (1 << 3);

	// Visibility from the left
	for(int y=0; y<h; ++y) {
		char tallest = no_tree;
		for(int x=0; x<w; ++x) {
			if(forest[y][x] > tallest) {
				visibility[y][x] |= vis_left;
				tallest = forest[y][x];
				if(tallest == '9') {
					// There cannot be taller trees, no sense in continuing.
					break;
				}
			}
		}
	}

	// Visibility from the right
	for(int y=0; y<h; ++y) {
		char tallest = no_tree;
		for(int x=w-1; x>=0; --x) {
			if(forest[y][x] > tallest) {
				visibility[y][x] |= vis_right;
				tallest = forest[y][x];
				if(tallest == '9') {
					// There cannot be taller trees, no sense in continuing.
					break;
				}
			}
		}
	}

	// Visibility from up
	for(int x=0; x<w; ++x) {
		char tallest = no_tree;
		for(int y=0; y<h; ++y) {
			if(forest[y][x] > tallest) {
				visibility[y][x] |= vis_up;
				tallest = forest[y][x];
				if(tallest == '9') {
					// There cannot be taller trees, no sense in continuing.
					break;
				}
			}
		}
	}

	// Visibility from down
	for(int x=0; x<w; ++x) {
		char tallest = no_tree;
		for(int y=h-1; y>=0; --y) {
			if(forest[y][x] > tallest) {
				visibility[y][x] |= vis_down;
				tallest = forest[y][x];
				if(tallest == '9') {
					// There cannot be taller trees, no sense in continuing.
					break;
				}
			}
		}
	}

	int num_visible = 0;
	for(auto &row : visibility) {
		for(auto v : row) {
			if(v != 0) {
				++num_visible;
			}
		}
	}

	std::cout << std::endl;
	std::cout << "How many trees are visible from outside the grid?" << std::endl;
	std::cout << "(Part 1): " << num_visible << std::endl;

	auto find_score = [&](int y0, int x0) -> int {
		const char t = forest[y0][x0];

		// std::cout << std::endl;
		// for(int y=0; y<h; ++y) {
		// 	for(int x=0; x<w; ++x) {
		// 		if(y == y0 || x == x0) {
		// 			std::cout << forest[y][x];
		// 		} else {
		// 			std::cout << ' ';
		// 		}
		// 	}
		// 	std::cout << std::endl;
		// }

		// Find score to the right
		int score_right = 1;
		for(int x=x0+1; x<w-1; ++x) {
			if(forest[y0][x] >= t) {
				break;
			}
			++score_right;
		}


		// Find score to the left
		int score_left = 1;
		for(int x=x0-1; x>=1; --x) {
			if(forest[y0][x] >= t) {
				break;
			}
			++score_left;
		}

		// Find score downwards
		int score_down = 1;
		for(int y=y0+1; y<h-1; ++y) {
			if(forest[y][x0] >= t) {
				break;
			}
			++score_down;
		}

		// Find score upwards
		int score_up = 1;
		for(int y=y0-1; y>=1; --y) {
			if(forest[y][x0] >= t) {
				break;
			}
			++score_up;
		}

		// std::cout << score_right * score_left * score_down * score_up << std::endl;

		return score_right * score_left * score_down * score_up;
	};

	int hi_score = 0;
	for(int y=1; y<h-1; ++y) {
		for(int x=1; x<w-1; ++x) {
			hi_score = std::max(hi_score, find_score(y, x));
		}
	}

	std::cout << std::endl;
	std::cout << "What is the highest scenic score possible for any tree?" << std::endl;
	std::cout << "(Part 2): " << hi_score << std::endl;

	return 0;
}