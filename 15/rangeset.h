#pragma once

#if !defined(RANGESET_H)

#include "range.h"
#include <list>
#include <iostream>

struct rangeset
{
	std::list<range> ranges;

	void insert(int x);
	void insert(range r);

	void remove(int x);
	void remove(range r);
};

std::ostream &operator<<(std::ostream &os, const rangeset &rs);

#endif // !defined(RANGESET_H)