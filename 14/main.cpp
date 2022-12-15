#include <fstream>
#include <string>
#include <iostream>
#include <set>
#include <climits>
#include <list>
#include <functional>
#include "gif.h"

struct point2
{
	int x, y;

	point2 &operator+=(const point2 &rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	point2 &operator-=(const point2 &rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	point2 operator+(const point2 &rhs) const {
		return point2{*this} += rhs;
	}

	point2 operator-(const point2 &rhs) const {
		return point2{*this} -= rhs;
	}

	bool operator==(const point2 &rhs) const {
		return x == rhs.x && y == rhs.y;
	}

	bool operator!=(const point2 &rhs) const {
		return !(*this == rhs);
	}

	point2 get_normal_manhattan() const {
		point2 normal{*this};
		if(normal.x != 0) {
			normal.x /= std::abs(normal.x);
		}
		if(normal.y != 0) {
			normal.y /= std::abs(normal.y);
		}
		return normal;
	}

	bool operator<(const point2 &rhs) const {
		if(x < rhs.x) {
			return true;
		} else if(x > rhs.x) {
			return false;
		}

		return (y<rhs.y);
	}
};

std::ostream &operator<<(std::ostream &os, const point2 &p) 
{
	return os << "(" << p.x << ", " << p.y << ")";	
}

struct grain
{
	int id;
	point2 pos;
};

int main(int argc, char **argv)
{
	if(argc != 2) {
		std::cout << "Usage: aoc14 <filename.txt>" << std::endl;
	}

	std::ifstream infile(argv[1], std::ios_base::in);
	if (!infile.is_open()) {
		return -1;
	}

	std::set<point2> rock_map;

	point2 origin{500, 0};
	point2 bb_min{origin};
	point2 bb_max{origin};

	auto update_bb = [&](const point2 &p) {
		if(bb_min.x > p.x) {
			bb_min.x = p.x;
		}
		if(bb_min.y > p.y) {
			bb_min.y = p.y;
		}

		if(bb_max.x < p.x) {
			bb_max.x = p.x;
		}
		if(bb_max.y < p.y) {
			bb_max.y = p.y;
		}
	};

	auto draw = [&](const point2 &p0, const point2 &p1) {
		point2 pdir = (p1-p0).get_normal_manhattan();
		for(point2 p=p0; p != p1; p += pdir) {
			if(rock_map.find(p) == rock_map.end()) {
				rock_map.insert(p);
			}
		}
	};

	auto parse_point2 = [&](const char *c, char **cend) -> point2 {
		point2 p0{0,0};
		char *end;
		p0.x = strtol(c, &end, 10);

		if(end == nullptr || *end == '\0' || *(end+1) == '\0') {
			if(cend != nullptr) {
				*cend = nullptr;
			}
			return p0;
		}

		c = end+1;
		p0.y = strtol(c, &end, 10);

		if(end != nullptr) {
			update_bb(p0);
		}
		if(cend != nullptr) {
			*cend = end;
		}
		return p0;
	};

	std::string line;
	while (std::getline(infile, line)) {
		std::cout << line << std::endl;

		const char *c = line.c_str();
		char *cend = nullptr;
		point2 p0 = parse_point2(c, &cend);
		while(*c != '\0') {
			point2 p1 = parse_point2(c, &cend);
			if(cend == nullptr) {
				break;
			}
			c = cend;
			draw(p0, p1);
			p0 = p1;

			for(int i=0; i<4 && *c != '\0'; ++i) {
				++c;
			}
		}
		if(rock_map.find(p0) == rock_map.end()) {
			rock_map.insert(p0);
		}
	}

	auto in_bounds = [bb_min, bb_max](const point2 &p) -> bool {
		if(p.x < bb_min.x || p.y < bb_min.y) {
			return false;
		}
		if(p.x > bb_max.x || p.y > bb_max.y) {
			return false;
		}
		return true;
	};

	std::set<point2> sand_map;

	auto print_map = [&](int floor) {
		for(int y=bb_min.y; y<=bb_max.y; ++y) {
			for(int x=bb_min.x; x<=bb_max.x; ++x) {
				if(y == floor) {
					std::cout << '#';
				} else {
					point2 p{x, y};
					if(rock_map.find(p) != rock_map.end()) {
						std::cout << '#';
					} else if(sand_map.find(p) != sand_map.end()) {
						std::cout << 'O';
					} else {
						std::cout << '.';
					}
				}
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	};

	print_map(INT_MAX);

	uint32_t rock = 0xFFB56300;
	uint32_t sediment = 0xFFDBAA72;
	uint32_t sand = 0xFFF9B05C;
	uint32_t foundation = 0xFF999999;

	GifWriter gif;
	point2 size = bb_max - bb_min + point2({1, 1});
	uint32_t *gif_buffer = new uint32_t[size.x * size.y];
	bool do_gif = GifBegin(&gif, "part1.gif", size.x, size.y, 1, 8, false);

	auto draw_frame = [&](const std::list<grain *> &grains) {
		if(do_gif) {
			memset(gif_buffer, 0xFF000000, sizeof(uint32_t) * size.x * size.y);
			for(auto r : rock_map) {
				point2 p = r - bb_min;
				gif_buffer[p.y * size.x + p.x] = rock;
			}
			for(auto s : sand_map) {
				point2 p = s - bb_min;
				gif_buffer[p.y * size.x + p.x] = sediment;
			}
			// int f = floor - bb_min.y;
			// if(f < size.y) {
			// 	for(int i=0; i<size.x; ++i) {
			// 		gif_buffer[f * size.x + i] = foundation;
			// 	}
			// }
			for(auto g : grains) {
				point2 p = g->pos - bb_min;
				gif_buffer[p.y * size.x + p.x] = sand;
			}

			GifWriteFrame(&gif, reinterpret_cast<uint8_t *>(gif_buffer), size.x, size.y, 1, 8, false);
		}
	};

	auto simulate = [&](std::function<bool(const point2 &)> blocked, std::function<bool(const point2 &)> stop_check, std::function<void(const std::list<grain *> &)> do_frame) -> int {
		sand_map.clear();

		int sand_id = 0;
		int stop_id = -1;
		std::list<grain *> grains;
		std::list<grain *> dead_grains;

		while(stop_id < 0) {
			grains.push_back(new grain{sand_id, origin});
			++sand_id;

			for(grain *g : grains) {
				point2 new_points[] = {
					g->pos + point2{0, 1},
					g->pos + point2{-1, 1},
					g->pos + point2{1, 1}
				};
				if(!blocked(new_points[0])) {
					g->pos = new_points[0];
				} else if(!blocked(new_points[1])) {
					g->pos = new_points[1];
				} else if(!blocked(new_points[2])) {
					g->pos = new_points[2];
				} else {
					sand_map.insert(g->pos);
					dead_grains.push_back(g);
				}
				if(stop_id == -1 && stop_check(g->pos)) {
					stop_id = g->id;
				}
			}
			while(!dead_grains.empty()) {
				grain *g = dead_grains.front();
				dead_grains.pop_front();
				grains.remove(g);
				delete g;
			}

			if(do_frame != nullptr) {
				do_frame(grains);
			}
		}

		while(!grains.empty()) {
			grain *g = dead_grains.front();
			grains.pop_front();
			delete g;
		}

		return stop_id;
	};

	auto blocked = [&rock_map, &sand_map](const point2 &p) -> bool {
		if(rock_map.find(p) != rock_map.end()) {
			return true;
		}

		if(sand_map.find(p) != sand_map.end()) {
			return true;
		}

		return false;
	};

	int abyss_id = simulate(
		blocked, 
		[&](const point2 &p){ return !in_bounds(p); }, 
		draw_frame);

	if(do_gif) {
		GifEnd(&gif);
	}
	delete [] gif_buffer;
	gif_buffer = nullptr;

	print_map(INT_MAX);

	std::cout << "How many units of sand come to rest before sand starts flowing into the abyss below?" << std::endl;
	std::cout << "Part 1) " << abyss_id << std::endl;

	int floor = bb_max.y + 2;
	update_bb({origin.x - floor, floor});
	update_bb({origin.x + floor, floor});

	size = bb_max - bb_min + point2({1, 1});
	gif_buffer = new uint32_t[size.x * size.y];
	do_gif = GifBegin(&gif, "part2.gif", size.x, size.y, 1, 8, false);

	auto draw_frame_floor = [&](const std::list<grain *> &grains) {
		memset(gif_buffer, 0xFF000000, sizeof(uint32_t) * size.x * size.y);
		for(auto r : rock_map) {
			point2 p = r - bb_min;
			gif_buffer[p.y * size.x + p.x] = rock;
		}
		for(auto s : sand_map) {
			point2 p = s - bb_min;
			gif_buffer[p.y * size.x + p.x] = sediment;
		}
		int f = floor - bb_min.y;
		if(f < size.y) {
			for(int i=0; i<size.x; ++i) {
				gif_buffer[f * size.x + i] = foundation;
			}
		}
		for(auto g : grains) {
			point2 p = g->pos - bb_min;
			gif_buffer[p.y * size.x + p.x] = sand;
		}

		GifWriteFrame(&gif, reinterpret_cast<uint8_t *>(gif_buffer), size.x, size.y, 1, 8, false);
	};

	int origin_id = simulate(
		[&](const point2 &p){ return p.y >= floor || blocked(p); }, 
		[origin](const point2 &p){ return p == origin; }, 
		draw_frame_floor);

	if(do_gif) {
		GifEnd(&gif);
	}
	delete [] gif_buffer;
	gif_buffer = nullptr;

	print_map(floor);

	std::cout << "Using your scan, simulate the falling sand until the source of the sand becomes blocked." << std::endl;
	std::cout << "How many units of sand come to rest?" << std::endl;
	std::cout << "Part 2) " << origin_id + 1 << std::endl;
}