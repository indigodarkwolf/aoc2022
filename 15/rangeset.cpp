#include "rangeset.h"
#include <iostream>

void rangeset::insert(int x) 
{
	insert({x,x});
}

// Merge all mergeable ranges into r, removing merged ranges from the list
static void condense_and_extract(range &r, std::list<range> &ranges)
{
	auto ri = ranges.begin();
	while(ri != ranges.end()) {
		auto rmerge = ri++;
		if(r.merge(*rmerge)) {
			ranges.erase(rmerge);
		}
	}
}

void rangeset::insert(range r)
{
	condense_and_extract(r, ranges);
	ranges.push_back(r);
}

void rangeset::remove(int x)
{
	remove({x,x});
}

void rangeset::remove(range r)
{
	range split = r;

	condense_and_extract(r, ranges);

	if(r.begin < split.begin) {
		ranges.push_back({r.begin, split.begin-1});
	}

	if(r.end > split.end) {
		ranges.push_back({split.end + 1, r.end});
	}
}

std::ostream &operator<<(std::ostream &os, const rangeset &rs)
{
	bool first = true;
	for(auto &r : rs.ranges) {
		if(!first) {
			std::cout << ",";
		}
		std::cout << "[" << r.begin << ", " << r.end << "]";
		first = false;
	}
	return os;
}