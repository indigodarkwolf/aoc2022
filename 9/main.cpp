#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <map>
#include <functional>
#include <set>

using point = std::tuple<int, int>;

std::set<point> Visited;
std::set<point> Visited10;

point Knots[10];

std::ostream &operator<<(std::ostream &os, const point &p)
{
	auto [x, y] = p;
	os << "(" << x << ", " << y << ")";
	return os;
}

point operator-(const point &p0, const point &p1)
{
	auto [x0, y0] = p0;
	auto [x1, y1] = p1;
	return std::make_tuple(x1-x0, y1-y0);
}

point operator+(const point &p0, const point &p1)
{
	auto [x0, y0] = p0;
	auto [x1, y1] = p1;
	return std::make_tuple(x1+x0, y1+y0);
}

int magnitude(const point &p)
{
	auto [x,y] = p;
	return std::max(std::abs(x), std::abs(y));
}

point cap(const point &p)
{
	auto [x,y] = p;
	if(x != 0) {
		x /= std::abs(x);
	}
	if(y != 0) {
		y /= std::abs(y);
	}
	return std::make_tuple(x, y);
}

void record(std::set<point> &set, const point &p)
{
	if(set.find(p) == set.end()) {
		set.insert(p);
	}
}

void move(const point &m)
{
	Knots[0] = Knots[0] + m;
	for(int i=0; i<9; ++i) {
		point &k0 = Knots[i];
		point &k1 = Knots[i+1];

		point diff = k1 - k0;
		if(magnitude(diff) > 1) {
			k1 = k1 + cap(diff);
		}
	}

	record(Visited, Knots[1]);
	record(Visited10, Knots[9]);
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
		if(line.length() > 0) {
			int dist = strtol(line.c_str() + 2, nullptr, 10);

			switch(toupper(line[0])) {
				case 'U':
					for(int i=0; i<dist; ++i) {
						move({0, 1});
					}
					break;
				case 'D':
					for(int i=0; i<dist; ++i) {
						move({0, -1});
					}
					break;
				case 'L':
					for(int i=0; i<dist; ++i) {
						move({-1, 0});
					}
					break;
				case 'R':
					for(int i=0; i<dist; ++i) {
						move({1, 0});
					}
					break;
			}
		}
	}

	std::cout << "How many positions does the tail of the rope visit at least once?" << std::endl;
	std::cout << "Part 1) " << Visited.size() << std::endl;

	std::cout << "How many positions does the tail of the rope visit at least once?" << std::endl;
	std::cout << "Part 2) " << Visited10.size() << std::endl;
	return 0;
}