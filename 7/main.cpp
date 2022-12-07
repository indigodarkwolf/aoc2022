#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <map>
#include <functional>

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

struct elf_file
{
	std::string name;
	int size;
};

struct elf_directory
{
	elf_directory(std::string _name, elf_directory *_parent) 
	:parent(_parent),
	name(_name)
	{ 
		if(parent == nullptr) {
			depth = 0;
		} else {
			depth = parent->depth + 1;
		}

		total_size = 0;
	}

	~elf_directory()
	{
		while (!files.empty()) {
			auto [name, f] = *files.begin();
			files.erase(name);
			delete f;
		}

		while (!subdirs.empty()) {
			auto [name, d] = *subdirs.begin();
			subdirs.erase(name);
			delete d;
		}
	}

	elf_directory *const parent;
	std::string name;

	int depth;

	std::map<std::string, elf_directory *> subdirs;
	std::map<std::string, elf_file *> files;

	int total_size;
};

std::ostream &operator<<(std::ostream &o, const elf_directory *d)
{
	if(d == nullptr) {
		o << "(nullptr)";
	} else if(d->parent != nullptr) {
		o << d->parent << " / " << d->name;
	} else {
		o << "/";
	}
	return o;
}

void for_each_directory(const elf_directory *root, std::function<void(const elf_directory *d)> f) {
	f(root);
	for(auto [name, d] : root->subdirs) {
		for_each_directory(d, f);
	}
}

void update_dir_sizes(elf_directory *d, int filesize)
{
	d->total_size += filesize;
	if(d->parent != nullptr) {
		update_dir_sizes(d->parent, filesize);
	}
}

int main()
{
	std::ifstream infile("input.txt", std::ios_base::in);
	if (!infile.is_open()) {
		return -1;
	}

	std::string line;

	elf_directory root("/", nullptr);

	elf_directory *cwd = &root;

	while (std::getline(infile, line)) {
		std::cout << line << std::endl;
		if(line.length() > 0) {
			std::vector<std::string> args = split(line, ' ');
			if(args[0] == "$") {
				if(args[1] == "cd") {
					// change directory
					if(args[2] == "/") {
						cwd = &root;
						// std::cout << "Change directory to: " << cwd << std::endl;
					} else if(args[2] == "..") {
						cwd = cwd->parent;
						if(cwd == nullptr) {
							cwd = &root;
						}
						// std::cout << "Change directory to: " << cwd << std::endl;
					} else {
						cwd = cwd->subdirs[args[2]];
						// std::cout << "Change directory to: " << cwd << std::endl;
					}
				} else if(args[1] == "ls") {
					// list contents
					// Nothing to do here, ignore. :3
				} else {
					// Nothing to do here, ignore. :3
				}
			} else if(args[0] == "dir") {
				cwd->subdirs.insert({args[1], new elf_directory(args[1], cwd)});
				// std::cout << "Add subdir: " << args[1] << " to " << cwd->name << std::endl;
			} else {
				int filesize = strtol(args[0].c_str(), nullptr, 10);
				cwd->files.insert({args[1], new elf_file{args[1], filesize}});
				update_dir_sizes(cwd, filesize);
				// std::cout << "Add file: " << args[1] << " (size=" << filesize << ")" << std::endl;
			}
		}
	}

	// Print the resulting tree
	for_each_directory(&root, [](const elf_directory *d) {
		for(int i=0; i<d->depth; ++i) {
			std::cout << "  ";
		}
		std::cout << "- " << d->name << " (dir, total size=" << d->total_size << ")" << std::endl;
		for(auto [name, f] : d->files) {
			for(int i=0; i<d->depth+1; ++i) {
				std::cout << "  ";
			}
			std::cout << "- " << f->name << " (file, size=" << f->size << ")" << std::endl;
		}		
	});

	// Find part 1 solution
	int sum_size_at_most_100000;
	for_each_directory(&root, [&sum_size_at_most_100000](const elf_directory *d) {
		if(d->total_size <= 100000) {
			// std::cout << d << " (total_size=" << d->total_size << ")" << std::endl;
			sum_size_at_most_100000 += d->total_size;
		}
	});

	std::cout << std::endl;
	std::cout << "The sum of the total sizes of directories with a total size of at most 100000: " << std::endl;
	std::cout << "(Part 1): " << sum_size_at_most_100000 << std::endl;

	int total_space_available = 70000000;
	int free_space = total_space_available - root.total_size;
	int space_needed = 30000000 - free_space;

	std::cout << std::endl;
	std::cout << "Max space: " << total_space_available << std::endl;
	std::cout << "Used space: " << root.total_size << std::endl;
	std::cout << "Free space: " << free_space << std::endl;
	std::cout << "Needed space: " << space_needed << std::endl;

	const elf_directory *smallest_to_free = &root;
	for_each_directory(&root, [space_needed, &smallest_to_free](const elf_directory *d) {
		if(d->total_size > space_needed && d->total_size < smallest_to_free->total_size) {
			smallest_to_free = d;
		}
	});

	std::cout << std::endl;
	std::cout << "The total size of the smallest directory that, if deleted, would free up enough space on the filesystem: " << std::endl;
	std::cout << "(Part 2): " << smallest_to_free->total_size << std::endl;

	return 0;
}