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

struct operation
{
	enum class op_type_t
	{
		add,
		sub,
		mul,
		div,
		sq
	} op_type;

	uint64_t value;
};

struct monkey
{
	std::queue<uint64_t> items;
	operation op;
	uint64_t test_value;
	int true_dest;
	int false_dest;
	uint64_t inspections;
};

int main()
{
	std::ifstream infile("input.txt", std::ios_base::in);
	if (!infile.is_open()) {
		return -1;
	}

	std::vector<monkey> monkeys;
	monkey *current = nullptr;

	std::string line;
	while (std::getline(infile, line)) {
		// std::cout << line << std::endl;
		if(line.length() > 0) {
			std::vector<std::string> args = split(line, ' ');

			// for(auto &arg : args) {
			// 	std::cout << "{" << arg << "}";
			// }
			// std::cout << std::endl;

			if(args[0] == "Monkey") {
				// std::cout << "Monkey " << monkeys.size() << ":" << std::endl;

				monkeys.push_back(monkey{});
				current = &monkeys.back();

				current->inspections = 0;

			} else if(args[3] == "items:") {
				// std::cout << "  Starting items: ";
				for(int i=4; i<args.size(); ++i) {
					current->items.push(strtol(args[i].c_str(), nullptr, 10));
					// std::cout << current->items.back() << ",";
				}
				// std::cout << std::endl;
			} else if(args[2] == "Operation:") {
				// std::cout << "  Operation: new = old ";
				switch(args[6][0]) {
					case '+':
						current->op.op_type = operation::op_type_t::add;
						// std::cout << "+ ";
						break;
					case '-':
						current->op.op_type = operation::op_type_t::sub;
						// std::cout << "- ";
						break;
					case '/':
						current->op.op_type = operation::op_type_t::div;
						// std::cout << "/ ";
						break;
					case '*':
						if(args[7] == "old") {
							// std::cout << "* old";
							current->op.op_type = operation::op_type_t::sq;
						} else {
							// std::cout << "* ";
							current->op.op_type = operation::op_type_t::mul;
						}
						break;
					default:
						break;
				}
				if(current->op.op_type != operation::op_type_t::sq) {
					current->op.value = strtol(args[7].c_str(), nullptr, 10);
					// std::cout << current->op.value << std::endl;
				}
			} else if(args[2] == "Test:") {
				current->test_value = strtol(args[5].c_str(), nullptr, 10);
				// std::cout << "  Test: divisible by " << current->test_value << std::endl;
			} else if(args[5] == "true:") {
				current->true_dest = strtol(args[9].c_str(), nullptr, 10);
				// std::cout << "    If true: throw to monkey " << current->true_dest << std::endl;
			} else if(args[5] == "false:") {
				current->false_dest = strtol(args[9].c_str(), nullptr, 10);
				// std::cout << "    If false: throw to monkey " << current->false_dest << std::endl;
			}
		}
	}

	{
		int m = 0;
		for(auto &monkey : monkeys) {
			std::cout << "Monkey " << m << ":" << std::endl;
			++m;
			std::cout << "  Starting items: (" << monkey.items.size() << ")" << std::endl;
			std::cout << "  Operation: new = old ";
			switch(monkey.op.op_type) {
				case operation::op_type_t::add:
					std::cout << "+ ";
					break;
				case operation::op_type_t::sub:
					std::cout << "- ";
					break;
				case operation::op_type_t::div:
					std::cout << "/ ";
					break;
				case operation::op_type_t::mul:
					std::cout << "* ";
					break;
				case operation::op_type_t::sq:
					std::cout << "* old" << std::endl;
					break;
				default:
					break;
			}
			if(monkey.op.op_type != operation::op_type_t::sq) {
				std::cout << monkey.op.value << std::endl;
			}
			std::cout << "  Test: divisible by " << monkey.test_value << std::endl;
			std::cout << "    If true: throw to monkey " << monkey.true_dest << std::endl;
			std::cout << "    If false: throw to monkey " << monkey.false_dest << std::endl;
		}
	}

	for(int rounds = 0; rounds < 10000; ++rounds) {
		if(rounds > 0 && (rounds % 1000 == 0)) {
			std::cout << "=== Round " << rounds << "===" << std::endl;
			for(int i=0; i<monkeys.size(); ++i) {
				std::cout << "Monkey " << i << " inspected items " << monkeys[i].inspections << " times." << std::endl;
			}			
		}
		int m = 0;
		for(auto &monkey : monkeys) {
			// std::cout << "Monkey " << m << ":" << std::endl;
			++m;
			while(!monkey.items.empty()) {
				uint64_t item = monkey.items.front();
				monkey.items.pop();

				// std::cout << "  Monkey inspects an item with a worry level of " << item << "." << std::endl;

				++monkey.inspections;

				switch(monkey.op.op_type) {
					case operation::op_type_t::add:
						item += monkey.op.value;
						// std::cout << "    Worry level increases by " << monkey.op.value << " to " << item << "." << std::endl;
						break;
					case operation::op_type_t::sub:
						item -= monkey.op.value;
						// std::cout << "    Worry level decreases by " << monkey.op.value << " to " << item << "." << std::endl;
						break;
					case operation::op_type_t::mul:
						item *= monkey.op.value;
						// std::cout << "    Worry level is multiplied by " << monkey.op.value << " to " << item << "." << std::endl;
						break;
					case operation::op_type_t::div:
						item /= monkey.op.value;
						// std::cout << "    Worry level is divided by " << monkey.op.value << " to " << item << "." << std::endl;
						break;
					case operation::op_type_t::sq:
						item *= item;
						// std::cout << "    Worry level is multiplied by itself to " << item << "." << std::endl;
						break;
					default:
						break;
				}

				// item /= 3;
				// std::cout << "    Monkey gets bored with item. Worry level is divided by 3 to " << item << "." << std::endl;

				item %= 9699690ULL;

				if(item % monkey.test_value) {
					// std::cout << "    Current worry level is not divisible by " << monkey.test_value << "." << std::endl;
					monkeys[monkey.false_dest].items.push(item);
					// std::cout << "    Item with worry level " << item << " is thrown to monkey " << monkey.false_dest << "." << std::endl;
				} else {
					// std::cout << "    Current worry level is divisible by " << monkey.test_value << "." << std::endl;
					monkeys[monkey.true_dest].items.push(item);
					// std::cout << "    Item with worry level " << item << " is thrown to monkey " << monkey.true_dest << "." << std::endl;
				}
			}
		}
	}

	std::priority_queue<uint64_t> inspections;
	for(int i=0; i<monkeys.size(); ++i) {
		inspections.push(monkeys[i].inspections);
		std::cout << "Monkey " << i << " inspected items " << monkeys[i].inspections << " times." << std::endl;
	}

	uint64_t most_inspections[2];
	most_inspections[0] = inspections.top();
	inspections.pop();
	most_inspections[1] = inspections.top();
	inspections.pop();

	uint64_t monkey_business = most_inspections[0] * most_inspections[1];

	std::cout << "What is the level of monkey business after 20 rounds of stuff-slinging simian shenanigans?" << std::endl;
	std::cout << "Part 1) " << monkey_business << std::endl;

	return 0;
}