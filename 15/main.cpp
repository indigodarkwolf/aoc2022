#include "point2.h"
#include "rangeset.h"

#include<set>
#include<string>
#include<vector>
#include<fstream>
#include<functional>
#include<iostream>
#include<stdlib.h>
#include<list>

struct sensor
{
	point2 pos;
	int beacon_dist;
};

struct input
{
	input(int argc, char **argv, std::function<void(const std::string &)> fn);
	std::vector<std::string> lines;
	int reference_line;
};

input::input(int argc, char **argv, std::function<void(const std::string &)> fn)
{
	if(argc != 3) {
		std::cout << "Usage: aoc14 <filename.txt> <line>" << std::endl;
		exit(-1);
	}

	std::ifstream infile(argv[1], std::ios_base::in);
	if (!infile.is_open()) {
		exit(-2);
	}

	reference_line = strtol(argv[2], nullptr, 10);

	std::string line;
	while (std::getline(infile, line)) {
		std::cout << line << std::endl;
		fn(line);
	}
	std::cout << std::endl;

	infile.close();
}

int main(int argc, char **argv)
{
	std::list<sensor> sensors;
	std::list<point2> beacons;
	input in(argc, argv, [&](const std::string &line) {
		const char *l = line.c_str() + 12; // strlen("Sensor at x=");
		char *end;

		sensor s;
		s.pos.x = strtol(l, &end, 10);
		l = end + 4; // strlen(", y=");

		s.pos.y = strtol(l, &end, 10);
		l = end + 25; // strlen(": closest beacon is at x=");

		point2 beacon_pos;

		beacon_pos.x = strtol(l, &end, 10);
		l = end + 4; // strlen(", y=");

		beacon_pos.y = strtol(l, &end, 10);
		l = end;

		s.beacon_dist = (beacon_pos - s.pos).get_magnitude_manhattan();
		sensors.push_back(s);
		beacons.push_back(beacon_pos);
	});

	sensors.sort([](const sensor &s0, const sensor &s1){
		return s0.pos < s1.pos;
	});

	rangeset excluded_positions;
	for(sensor &s : sensors) {
		const int dist_to_y = std::abs(in.reference_line - s.pos.y);
		const int excluded_dist = s.beacon_dist - dist_to_y;
		if(excluded_dist < 0) {
			continue;
		}
		const int startx = s.pos.x - excluded_dist;
		const int endx = s.pos.x + excluded_dist;
		excluded_positions.insert({startx, endx});
	}

	for(point2 &p : beacons) {
		if(p.y == in.reference_line) {
			excluded_positions.remove(p.x);
		}
	}

	int num_excluded = 0;
	for(auto &r : excluded_positions.ranges) {
		num_excluded += r.width();
	}
	std::cout << "In the row where y=2000000, how many positions cannot contain a beacon?" << std::endl;
	std::cout << "Part 1) " << num_excluded << std::endl;
	std::cout << std::endl;

	std::set<int> dl_lines;
	std::set<int> dr_lines;
	std::set<point2> test_points;

	auto add_test_point = [](const point2 &p, std::set<point2> &test_points) {
		if(p.x < 0 || p.x > 4000000) {
			return;
		}

		if(p.y < 0 || p.y > 4000000) {
			return;
		}

		if(test_points.find(p) == test_points.end()) {
			test_points.insert(p);
		}
	};

	for(auto si = sensors.begin(); si != sensors.end(); ++si) {
		auto sj = si;
		for(++sj; sj != sensors.end(); ++sj) {
			if(si->pos.y == sj->pos.y) {
				add_test_point({si->pos.x + si->beacon_dist + 1, si->pos.y}, test_points);
			} else if(si->pos.x == sj->pos.x) {
				add_test_point({si->pos.x, si->pos.y + si->beacon_dist + 1}, test_points);
			} else if(si->pos.x < sj->pos.x) {
				dl_lines.insert(si->pos.x + si->pos.y + si->beacon_dist + 1);
			} else {
				dr_lines.insert(si->pos.x - si->pos.y - si->beacon_dist - 1);
			}
		}
	}

	for(int dl : dl_lines) {
		for(int dr : dr_lines) {
			int xx = dr + dl;
			if(xx  % 0x1) {
				// This intersection cannot fall on an integer coordinate if xx is odd.
				continue;
			}
			int yy = dl - dr; // The same rule would apply to yy, but we don't need to check.
			add_test_point({xx >> 1, yy >> 1}, test_points);
		}
	}

	auto any_sensor_can_see = [](const point2 &p, std::list<sensor> &sensors) {
		for(const sensor &s : sensors) {
			if((s.pos - p).get_magnitude_manhattan() <= s.beacon_dist) {
				return true;
			}
		}
		return false;
	};

	std::cout << "Find the only possible position for the distress beacon." << std::endl;
	std::cout << "What is its tuning frequency?" << std::endl;
	for(const point2 &p : test_points) {
		if(!any_sensor_can_see(p, sensors)) {
			std::cout << "Part 2) " << (uint64_t)p.x * 4000000ULL + (uint64_t)p.y << std::endl;
			break;
		}
	}
	return 0;
}