#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <map>
#include <functional>
#include <queue>
#include <set>
#include <climits>

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

struct point3
{
	int x,y,z;

	// Math operations

	point3 &operator+=(const point3 &p)
	{
		x += p.x;
		y += p.y;
		z += p.z;
		return *this;
	}

	point3 &operator-=(const point3 &p)
	{
		x -= p.x;
		y -= p.y;
		z -= p.z;
		return *this;
	}

	point3 operator+(const point3 &p) const
	{
		return point3(*this) += p;
	}

	point3 operator-(const point3 &p) const
	{
		return point3(*this) -= p;
	}

	// Magnitude

	int mag_manhatten() const
	{
		return std::abs(x) + std::abs(y) + std::abs(z);
	}

	int mag_squared() const
	{
		return x*x + y*y + z*z;
	}

	// Sort
	bool operator<(const point3 &p) const {
		if (x < p.x) {
			return true;
		} else if(x > p.x) {
			return false;
		}

		if(y < p.y) {
			return true;
		} else if(y > p.y) {
			return false;
		}

		if(z < p.z) {
			return true;
		}
		return false;		
	}
};

struct path
{
	const path *parent;
	int depth;
	point3 pos;
};

int main()
{
	std::ifstream infile("input.txt", std::ios_base::in);
	if (!infile.is_open()) {
		return -1;
	}

	std::vector<std::string> map;

	std::string line;
	while (std::getline(infile, line)) {
		std::cout << line << std::endl;
		if(line.length() > 0) {
			map.push_back(line);
		}
	}
	std::cout << std::endl;

	const int h = map.size();
	const int w = map[0].length();

	point3 start, end;
	std::list<point3> starts;

	for(int y=0; y<h; ++y) {
		for(int x=0; x<w; ++x) {
			if(map[y][x] == 'S') {
				start = {x, y, 'a'};
				map[y][x] = 'a';
			} else if(map[y][x] == 'E') {
				end = {x, y, 'z'};
				map[y][x] = 'z';
			} else if(map[y][x] == 'a') {
				starts.push_front({x, y, 'a'});
			}
		}
	}

	auto height = [&map, w, h](int y, int x) -> int {
		if(x < 0 || x >= w) { 
			return INT_MAX;
		}
		if(y < 0 || y >= h) {
			return INT_MAX;
		}
		return map[y][x];
	};

	auto pathfind_distance = [&map, height, w, h](point3 start, point3 end) -> int {
		std::queue<const path *> open;
		std::set<point3> visited;

		open.push(new path{nullptr, 0, start});
		visited.insert(start);

		const path *solution = nullptr;
		while(!open.empty()) {
			const path *current = open.front();
			open.pop();

			auto [x, y, z] = current->pos;

			point3 steps[4] = {
				{ x-1, y, height(y, x-1) },
				{ x+1, y, height(y, x+1) },
				{ x, y-1, height(y-1, x) },
				{ x, y+1, height(y+1, x) }
			};

			for(point3 &step : steps) {
				bool is_legal = ((step.z - z) <= 1);
				is_legal = is_legal && (step.x < w);
				is_legal = is_legal && (step.y < h);
				is_legal = is_legal && (step.x >= 0);
				is_legal = is_legal && (step.y >= 0);
				is_legal = is_legal && (visited.find(step) == visited.end());

				if(is_legal) {
					const path *new_path = new path{current, current->depth+1, step};
					open.push(new_path);
					visited.insert(step);

					if((new_path->pos - end).mag_manhatten() == 0) {
						solution = new_path;
						break;
					}
				}
			}

			if(solution != nullptr) {
				break;
			}
		}

		if(solution != nullptr) {
			return solution->depth;
		}

		return INT_MAX;
	};

	point3 shortest_start = start;
	int shortest_distance = pathfind_distance(start, end);

	std::cout << "What is the fewest steps required to move from your current position to the" << std::endl;
	std::cout << "location that should get the best signal?" << std::endl;
	std::cout << "Part 1) " << shortest_distance << std::endl;
	std::cout << std::endl;

	for(point3 s : starts) {
		int d = pathfind_distance(s, end);
		if(d < shortest_distance) {
			shortest_start = s;
			shortest_distance = d;
		}
	}

	std::cout << "What is the fewest steps required to move starting from any square with" << std::endl;
	std::cout << "elevation a to the location that should get the best signal?" << std::endl;
	std::cout << "Part 2) " << shortest_distance << std::endl;
	std::cout << std::endl;

	return 0;
}