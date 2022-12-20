#include "point2.h"
#include "rangeset.h"
#include "graph.h"

#include<set>
#include<string>
#include<vector>
#include<fstream>
#include<functional>
#include<iostream>
#include<stdlib.h>
#include<list>
#include<sstream>
#include<map>
#include<queue>
#include<stack>
#include<sstream>

struct sensor
{
	point2 pos;
	int beacon_dist;
};

struct input
{
	input(int argc, char **argv, std::function<void(const std::string &)> fn);
	std::vector<std::string> lines;
};

input::input(int argc, char **argv, std::function<void(const std::string &)> fn)
{
	if(argc != 2) {
		std::cout << "Usage: aoc14 <filename.txt>" << std::endl;
		exit(-1);
	}

	std::ifstream infile(argv[1], std::ios_base::in);
	if (!infile.is_open()) {
		exit(-2);
	}

	std::string line;
	while (std::getline(infile, line)) {
		std::cout << line << std::endl;
		fn(line);
	}
	std::cout << std::endl;

	infile.close();
}

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

struct valve_id_t
{
	constexpr valve_id_t():id(0) {}
	constexpr valve_id_t(uint16_t _id):id(_id) {}
	constexpr valve_id_t(const char *_id):id(*reinterpret_cast<const uint16_t *>(_id)) {}
	constexpr valve_id_t(const char(&_id)[2]):id(*reinterpret_cast<const uint16_t *>(_id)) {}

	constexpr operator uint16_t() const { return id; }

	constexpr bool operator==(const valve_id_t &rhs) { return id == rhs.id; }

	uint16_t id;
};

std::ostream &operator<<(std::ostream &os, const valve_id_t &v) 
{
	const char *id = reinterpret_cast<const char *>(&v.id);
	os << id[0] << id[1];
	return os;
}

struct valve
{
	valve(const std::string &line)
	{
		const char *l = line.c_str();
		const char *lend = l + line.length();
		valve_id = l + 6; // strlen("Valve ")

		index = -1;

		char *e = nullptr;
		flow_rate = strtol(l + 23, &e, 10); // strlen("Valve XX has flow rate=")

		char const *exit_list = e + 25; // strlen("; tunnels lead to valves ")
		if(*(exit_list+1) == '\0') {
			--exit_list; // must've been "; tunnel leads to valve "
		}
		while(exit_list < lend) {
			exits.push_back(new valve_exit{exit_list, 1});
			exit_list += 4;
		}
	}

	valve_id_t valve_id;
	int index;

	int flow_rate;

	struct valve_exit
	{
		valve_id_t valve_id;
		int distance;
	};

	std::list<valve_exit *> exits;

	void substitute_exit(valve_id_t exit_id, const std::list<valve_exit *> &exits);
};

std::ostream &operator<<(std::ostream &os, const valve &v)
{
	os << "Valve " << v.valve_id << " has flow rate=" << v.flow_rate << "; tunnels lead to valves ";
	bool first = true;
	for(auto e : v.exits) {
		if(!first) {
			os << ", ";
		}
		os << e->valve_id << "(" << e->distance << ")";
		first = false;
	}
	return os;
}

std::ostream &operator<<(std::ostream &os, const std::list<valve::valve_exit *> &exits)
{
	bool first = true;
	for(auto new_exit : exits) {
		if(!first) {
			os << ", ";
		}
		os << new_exit->valve_id;
		first = false;
	}
	return os;
}

void valve::substitute_exit(valve_id_t exit_id, const std::list<valve_exit *> &new_exits)
{
	std::cout << "Valve " << valve_id << " substituting " << exit_id << " with " << new_exits << std::endl;
	std::cout << "\twas containing " << exits << std::endl;

	int cost = 0;
	for(auto exit_iter = exits.begin(); exit_iter != exits.end(); ++exit_iter) {
		if((*exit_iter)->valve_id == exit_id) {
			cost = (*exit_iter)->distance;
			exits.erase(exit_iter);
			break;
		}
	}

	for(auto new_exit : new_exits) {
		if(new_exit->valve_id != valve_id) {
			exits.push_back(new valve_exit{new_exit->valve_id, new_exit->distance + cost});
		}
	}

	std::cout << "\tnow containing " << exits << std::endl;
}

struct state
{
	state(valve_id_t loc);
	state(const state &src, valve_id_t new_location, int time_elapsed, int max_time);

	const state *parent;

	valve_id_t location;
	int time;

	int total_flow_rate;
	int total_flow;
	std::set<valve_id_t> open_valves;
};

state::state(valve_id_t loc)
	:parent(nullptr),
	location(loc),
	time(0),
	total_flow_rate(0),
	total_flow(0),
	open_valves()
{ }

state::state(const state &src, valve_id_t new_location, int time_elapsed, int max_time)
	:parent(&src),
	location(new_location),
	time(std::min(src.time + time_elapsed, max_time)),
	total_flow_rate(src.total_flow_rate),
	total_flow(src.total_flow + src.total_flow_rate * (time - src.time)),
	open_valves(src.open_valves)
{ 
	if(time_elapsed < 0) throw std::out_of_range("param 'time_elapsed' is out of range");
	if(time < 0) throw std::out_of_range("member 'time' has been created out of range");
}

std::ostream &operator<<(std::ostream &os, const state &s) {
	os << "== Minute " << s.time << " ==" << std::endl;
	if(s.open_valves.size() > 1) {
		os << "Valves ";
		bool first = true;
		for(auto v : s.open_valves) {
			if(!first) {
				os << " and ";
			}
			os << v;
			first = false;
		}
		os << " are open, releasing " << s.total_flow_rate << " pressure." << std::endl;
	} else if(s.open_valves.size() == 1) {
		os << "Valve " << *s.open_valves.begin() << " is open, releasing " << s.total_flow_rate << " pressure." << std::endl;
	} else {
		os << "No valves are open." << std::endl;
	}
	return os;
}

struct simulation_params
{
	valve_id_t homepoint;
	std::map<valve_id_t, valve *> &valves;
	weighted_graph &valve_graph;
	uint64_t total_calls;
};

int find_max_flow(uint16_t allowed_nodes, int max_time, const state &s0, simulation_params &params)
{
	++params.total_calls;
	int most_flow = 0;

	if(s0.time >= max_time) {
		return s0.total_flow;
	} else {
		{
			state s(s0, s0.location, max_time - s0.time, max_time);
			const int flow = find_max_flow(allowed_nodes, max_time, s, params);
			most_flow = std::max(most_flow, flow);
		}

		auto current_valve = params.valves[s0.location];
		if(current_valve == nullptr) {
			std::cout << "!! Error finding valve " << s0.location << std::endl;
			return most_flow;
		}

		if(s0.location != params.homepoint && s0.open_valves.find(s0.location) == s0.open_valves.end()) {
			state s(s0, s0.location, 1, max_time);
			s.open_valves.insert(s0.location);
			s.total_flow_rate += current_valve->flow_rate;
			const int flow = find_max_flow(allowed_nodes, max_time, s, params);
			most_flow = std::max(most_flow, flow);
		} else {
			for(auto [valve_id, v] : params.valves) {
				if(valve_id != params.homepoint && (allowed_nodes & (1 << v->index)) && s0.open_valves.find(valve_id) == s0.open_valves.end()) {
					int distance = params.valve_graph.get_shortest_path_length(current_valve->index, v->index);
					if(s0.time + distance < max_time) {
						state s(s0, valve_id, distance, max_time);
						const int flow = find_max_flow(allowed_nodes, max_time, s, params);
						most_flow = std::max(most_flow, flow);
					}
				}
			}
		}
	}

	return most_flow;
}

int main(int argc, char **argv)
{
	std::map<valve_id_t, valve *> valves;

	input in(argc, argv, [&](const std::string &line) {
		valve *v = new valve(line);
		valves.insert({v->valve_id, v});
	});

	valve_id_t homepoint("AA");

	// Simplify the valve representation by removing valves with a flow rate of 0.
	for(auto viter = valves.begin(); viter != valves.end(); ++viter) {
	check_valve:
		auto [valve_id, v] = *viter;
		if(v->valve_id != homepoint && v->flow_rate == 0) {
			for(auto e : v->exits) {
				if(e == nullptr) {
					std::cout << "!! nullptr exit valve inserted into " << v->valve_id << "!!" << std::endl;
				}
				valve *exit_valve = valves[e->valve_id];
				if(exit_valve == nullptr) {
					std::cout << "!! Error finding exit valve " << e->valve_id << "!!" << std::endl;
					std::cout << "!! e == " << e << "!!" << std::endl;
					std::cout << "!! v == " << v << std::endl;
					std::cout << "!! v->exits == " << v->exits << std::endl;
				}
				exit_valve->substitute_exit(valve_id, v->exits);

				for(auto x : exit_valve->exits) {
					if(valves.find(x->valve_id) == valves.end()) {
						std::cout << "!! Error while substituting " << valve_id << "!!" << std::endl;
					}
				}
			}
			valves.erase(viter++);
			if(viter != valves.end()) {
				goto check_valve;
			} else {
				break;
			}
		}
	}
	std::cout << std::endl;

	for(int index = 0; auto [valve_id, v] : valves) {
		v->index = index++;
	}

	for(int index = 0; auto [valve_id, v] : valves) {
		std::cout << valve_id << " => index " << v->index << std::endl;
	}
	std::cout << std::endl;

	std::cout << "Building nav graph..." << std::endl;
	weighted_graph valve_graph(valves.size());

	for(auto [valve_id, v] : valves) {
		for(auto x : v->exits) {
			valve_graph.add_edge(v->index, valves[x->valve_id]->index, x->distance);
		}
	}
	valve_graph.calculate_shortest_path_lengths();
	std::cout << valve_graph << std::endl;
	std::cout << std::endl;

	simulation_params params{homepoint, valves, valve_graph, 0};
	state s(params.homepoint);

	// Simulation of part 1

	std::cout << "Work out the steps to release the most pressure in 30 minutes." << std::endl;
	std::cout << "What is the most pressure you can release?" << std::endl;
	std::cout << "Part 1) " << find_max_flow(0xffff, 30, s, params) << std::endl;
	std::cout << std::endl;

	std::cout << "In " << params.total_calls << " iterations." << std::endl;
	std::cout << std::endl;

	int max_flow = 0;
	for(uint16_t i=0; i<0x8000; ++i) {
		max_flow = std::max(max_flow, find_max_flow(i, 26, s, params) + find_max_flow(~i, 26, s, params));
		if(i % 0x100 == 0) {
			std::cout << i << " iterations, max flow " << max_flow << std::endl;
		}
	}
	std::cout << "Part 2) " << max_flow << std::endl;

	return 0;
}