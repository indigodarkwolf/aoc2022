#include "point2.h"

#include <cmath>

// Mutating math ops
point2 &point2::operator+=(const point2 &rhs) {
	x += rhs.x;
	y += rhs.y;
	return *this;
}
point2 &point2::operator-=(const point2 &rhs) {
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

point2 &point2::operator*=(int scalar) {
	x *= scalar;
	y *= scalar;
	return *this;
}

point2 &point2::operator/=(int scalar) {
	x /= scalar;
	y /= scalar;
	return *this;
}

// Copy math ops
point2 point2::operator+(const point2 &rhs) const {
	return point2{*this} += rhs;
}

point2 point2::operator-(const point2 &rhs) const {
	return point2{*this} -= rhs;
}

point2 point2::operator*(int scalar) const {
	return point2{*this} *= scalar;
}

point2 point2::operator/(int scalar) const {
	return point2{*this} /= scalar;
}

// Equality
bool point2::operator==(const point2 &rhs) const {
	return x == rhs.x && y == rhs.y;
}

bool point2::operator!=(const point2 &rhs) const {
	return !(*this == rhs);
}

// Others
int point2::get_magnitude_manhattan() const {
	return std::abs(x) + std::abs(y);
}

point2 point2::get_normal_manhattan() const {
	point2 normal{*this};
	if(normal.x != 0) {
		normal.x /= std::abs(normal.x);
	}
	if(normal.y != 0) {
		normal.y /= std::abs(normal.y);
	}
	return normal;
}

// Sorting
std::strong_ordering point2::operator<=>(const point2 &rhs) const {
	if(y < rhs.y) {
		return std::strong_ordering::less;
	} else if(y > rhs.y) {
		return std::strong_ordering::greater;
	}

	return (x <=> rhs.x);
}
