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

struct object
{
	enum class object_type
	{
		list,
		number
	} type;

	object(object_type t) : type(t) { };
	virtual ~object() {};
};

struct number : object
{
	number(int n) : object(object::object_type::number), value(n) { };
	~number() { };

	int value;
};

struct list : object
{
	list() : object(object::object_type::list) { };
	list(int value) : object(object::object_type::list) { 
		contents.push_back(new number(value));
	}
	list(list *item) : object(object::object_type::list) { 
		contents.push_back(item);
	};
	~list() {
		while(!contents.empty()) {
			object *o = contents.back();
			contents.pop_back();
			delete o;
		}
	}

	std::vector<object *> contents;
};

std::ostream &operator<<(std::ostream &os, const object *obj)
{
	if(obj == nullptr) {
		os << "(nullptr)";
	} else {
		if(obj->type == object::object_type::number) {
			os << static_cast<const number *>(obj)->value;
		} else {
			os << '[';
			const list *l = static_cast<const list *>(obj);
			bool front = true;
			for(const object *o : l->contents) {
				if(!front) {
					os << ',';
				}
				os << o;
				front = false;
			}
			os << ']';
		}
	}

	return os;
}

object *parse_element(const char *&e) 
{
	switch(*e) {
		case '[': {
			list *l = new list();
			++e;
			while(*e != ']' && *e != '\0') {
				l->contents.push_back(parse_element(e));
				while(*e == ',' || *e == ' ') {
					++e;
				}
			}
			++e;
			return l;
		}
		default: {
			char *end;
			int value = strtol(e, &end, 10);
			number *n = new number(value);
			e = end;
			return n;
		}
	}
}

object *parse_line(const char *line)
{
	return parse_element(line);
}

enum class pair_order
{
	in_order,
	not_in_order,
	ambiguous
};

pair_order pair_get_order(const object *p0, const object *p1)
{
	if(p0->type != p1->type) {
		if(p0->type == object::object_type::number) {
			list *temp = new list(static_cast<const number *>(p0)->value);
			pair_order in_order = pair_get_order(temp, p1);
			delete temp;
			return in_order;
		} else {
			list *temp = new list(static_cast<const number *>(p1)->value);
			pair_order in_order = pair_get_order(p0, temp);
			delete temp;
			return in_order;
		}
	} else if(p0->type == object::object_type::number) {
		if(static_cast<const number *>(p0)->value < static_cast<const number *>(p1)->value) {
			return pair_order::in_order;
		} else if(static_cast<const number *>(p0)->value > static_cast<const number *>(p1)->value) {
			return pair_order::not_in_order;
		} else {
			return pair_order::ambiguous;
		}
	}

	int i=0;
	pair_order in_order = pair_order::ambiguous;

	const list *left = static_cast<const list *>(p0);
	const list *right = static_cast<const list *>(p1);
	int max_i = std::min(left->contents.size(), right->contents.size());
	while(in_order == pair_order::ambiguous && i < max_i) {
		if(left->contents.size() <= i && right->contents.size() > i) {
			in_order = pair_order::in_order;
		} else if(left->contents.size() > i && right->contents.size() <= i) {
			in_order = pair_order::not_in_order;
		} else {
			in_order = pair_get_order(left->contents[i], right->contents[i]);
		}
		++i;
	}
	if(in_order == pair_order::ambiguous) {
		if(left->contents.size() < right->contents.size()) {
			return pair_order::in_order;
		} else if(right->contents.size() < left->contents.size()) {
			return pair_order::not_in_order;
		}
	}
	return in_order;
}

int main(int argc, char **argv)
{
	if(argc != 2) {
		std::cout << "Usage: aoc input.txt" << std::endl;
	}

	std::ifstream infile(argv[1], std::ios_base::in);
	if (!infile.is_open()) {
		return -1;
	}

	std::vector<const object *> packets;

	int ordered_sum = 0;
	std::string line;
	while (std::getline(infile, line)) {
		std::cout << line << std::endl;
		if(line.length() > 0) {
			packets.push_back(parse_line(line.c_str()));

			if(packets.size() % 2 == 0) {
				if(pair_get_order(packets[packets.size()-2], packets[packets.size()-1]) != pair_order::not_in_order) {
					ordered_sum += (packets.size() >> 1);
				}
			}
		}
	}
	std::cout << std::endl;

	std::cout << "Determine which pairs of packets are already in the right order." << std::endl;
	std::cout << "What is the sum of the indices of those pairs?" << std::endl;
	std::cout << "Part 1) " << ordered_sum << std::endl;
	std::cout << std::endl;

	object *decoder_packets[] = {
		new list(new list(2)),
		new list(new list(6))
	};

	for(object *p : decoder_packets) {
		packets.push_back(p);
	}

	std::sort(packets.begin(), packets.end(), [](const object *a, const object *b){ return (pair_get_order(a, b) != pair_order::not_in_order); });

	int decoder_key = 1;
	int decoder_index = 1;
	for(const object *p : packets) {
		std::cout << p << std::endl;

		for(const object *d : decoder_packets) {
			if(p == d) {
				decoder_key *= decoder_index;
			}
		}

		++decoder_index;
	}
	std::cout << std::endl;

	std::cout << "Organize all of the packets into the correct order." << std::endl;
	std::cout << "What is the decoder key for the distress signal?" << std::endl;
	std::cout << "Part 2) " << decoder_key << std::endl;
	std::cout << std::endl;

	return 0;
}