#include "range.h"

#include <cmath>

int range::width() const
{
	return 1 + end - begin;
}

std::strong_ordering range::operator<=>(const range &rhs) const
{
	if(std::strong_ordering o = (begin <=> rhs.begin); o != std::strong_ordering::equal) {
		return o;
	}
	return end <=> rhs.end;
}

bool range::contains(int x) const
{
	return (begin >= x) && (end <= x);
}

int range::num_contained(const range &rhs) const
{
	if(rhs < *this) {
		return rhs.num_contained(*this);
	} else if(begin == rhs.begin) {
		return 1 + std::min(end, rhs.end) - begin;
	}

	return std::max(1 + end - rhs.begin, 0);
}

bool range::adjacent(const range &rhs) const
{
	return (rhs.end - begin == 1) || (end - rhs.begin == 1);
}

bool range::merge(const range &r)
{
	if(adjacent(r) || (num_contained(r) > 0)) {
		begin = std::min(begin, r.begin);
		end = std::max(end, r.end);
		return true;
	}

	return false;
}

std::ostream &operator<<(std::ostream &os, const range &r)
{
	return (os << '[' << r.begin << ", " << r.end << ']');
}