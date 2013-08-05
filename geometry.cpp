#include <math.h>
#include <sstream>
#include "geometry.h"
#define ERROR_MARGIN .01

std::string pointToString(point const &p) {
	std::ostringstream ss;
	ss << "(" << p.x << ", " << p.y << ")";
	return ss.str();

}

std::string circleToString(circle const &c) {
	std::ostringstream ss;
	ss << "center: " << pointToString(c.center) << ", radius: " << c.radius;
	return ss.str();
}

inline bool almostEqual(number a, number b) {
	return delta(a, b) < ERROR_MARGIN;
}

inline number sqr(number a) {
	return a * a;
}

inline number delta(number a, number b) {
	return fabs(a - b);
}

bool inside(const rectangle & a, const point & b) {
	return a.bottomleft.x <= b.x && b.x < a.topright.x &&
		   a.bottomleft.y <= b.y && b.y < a.topright.y;
}

inline point between(point a, point b) {
	point average = {a.x + b.x / 2, a.y + b.y / 2};
	return average;
}

point average(std::vector<point> list) {
	number sumX, sumY;
	for(std::vector<point>::iterator it = list.begin(); it != list.end(); ++it)
		sumX += it->x, sumY += it-> y;
	point avg = {sumX / list.size(), sumY / list.size()};
	return avg;
}

number distance(point const &a,  const point &b) {
	// d = sqrt(dx^2 + dy^2)
	return hypot(a.x - b.x, a.y - b.y);
	//return sqrt(sqr(a.x - b.x) + sqr(a.y - b.y));
}

bool operator==(const point& lhs, const point& rhs) {
	return (almostEqual(lhs.x, rhs.x) && almostEqual(lhs.y, rhs.y));
}

bool operator==(const circle& lhs, const circle& rhs) {
	return (lhs.center == rhs.center && almostEqual(lhs.radius, rhs.radius));
}

bool operator==(const rectangle& lhs, const rectangle& rhs) {
	return (lhs.bottomleft == rhs.bottomleft && lhs.topright == rhs.topright);
}

bool operator!=(const point& lhs, const point& rhs) {
	return ! (lhs == rhs);
}

bool operator!=(const circle& lhs, const circle& rhs) {
	return ! (lhs == rhs);
}

bool operator!=(const rectangle& lhs, const rectangle& rhs) {
	return ! (lhs == rhs);
}

std::vector<point> bilateral(circle const &a, circle const &b) {
	number ah, dx, dy, d, h, rx, ry;
	number x2, y2;

	// dx and dy are the vertical and horizontal distances between the circle
	// centers.
	dx = b.center.x - a.center.x;
	dy = b.center.y - a.center.y;

	// Determine the straight-line distance between the centers.
	d = hypot(dx,dy); // Suggested by Keith Briggs

	// Check for solvability.
	if (d >= (a.radius + b.radius)) {
		// halfway solution. circles do not intersect.
		// Triangle d, dy, and dx is simmilar to new triangle with this ratio
		number aratio = a.radius / d;
		number bratio = b.radius / d;

		// af is the farthest point reached by circle a toward circle b
		// df is the farthest point reached by circle b toward circle a
		point af = {aratio * dx, aratio * dy}, bf = {bratio * dx, bratio * dy},
				avg = between(af, bf);


		//return list
		std::vector<point> ret;
		ret.push_back(avg);
		ret.push_back(avg);
		return ret;
	}

	if (d < delta(a.radius, b.radius)) {
		// no solution. one circle is contained in the other
		std::vector<point> ret;
		return ret;
	}

	// 'point 2' is the point where the line through the circle
	// intersection points crosses the line between the circle
	// centers.

	// Determine the distance from point 0 to point 2.
	ah = (sqr(a.radius) - sqr(b.radius) + (d*d)) / (2.0 * d) ;

	// Determine the relative coordinates of point 2.
	x2 = a.center.x + (dx * ah/d);
	y2 = a.center.y + (dy * ah/d);

	// Determine the distance from point 2 to any intersection point.
	h = sqrt(sqr(a.radius) - (ah*ah));

	// Now determine the offsets of the intersection points from point 2.
	rx = -dy * (h/d);
	ry = dx * (h/d);

	// Determine the absolute intersection points.
	point solution1 = {x2 + rx, y2 + ry},
		  solution2 = {x2 - rx, y2 - ry};

	// Return list of solutions as a vector
	std::vector<point> ret;
	ret.push_back(solution1);
	ret.push_back(solution2);
	return ret;

}


