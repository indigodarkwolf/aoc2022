#pragma once

#include <compare>
#include <iostream>

struct range
{
	int begin;
	int end;

	int width() const;

	std::strong_ordering operator<=>(const range &rhs) const;

	bool contains(int x) const;
	int num_contained(const range &rhs) const;
	bool adjacent(const range &rhs) const;

	bool merge(const range &r);
};

std::ostream &operator<<(std::ostream &os, const range &r);
