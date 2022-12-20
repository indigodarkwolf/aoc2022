#pragma once

#include<compare>

struct point2
{
	int x, y;

	// Mutating math ops
	point2 &operator+=(const point2 &rhs);
	point2 &operator-=(const point2 &rhs);
	point2 &operator*=(int scalar);
	point2 &operator/=(int scalar);

	// Copy math ops
	point2 operator+(const point2 &rhs) const;
	point2 operator-(const point2 &rhs) const;
	point2 operator*(int scalar) const;
	point2 operator/(int scalar) const;

	// Equality
	bool operator==(const point2 &rhs) const;
	bool operator!=(const point2 &rhs) const;

	// Others
	int get_magnitude_manhattan() const;
	point2 get_normal_manhattan() const;

	// Sorting
	std::strong_ordering operator<=>(const point2 &rhs) const;
};
