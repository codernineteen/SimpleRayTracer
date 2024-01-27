#pragma once
#include "utilities.h"

class Interval
{
public:
	double min, max;

	Interval() : min(+infinity), max(-infinity) {} // default is empty
	Interval(double _min, double _max) : min(_min) , max(_max) {}

	bool contains(double x) const { return min <= x && x <= max; }
	bool surrounds(double x) const { return min < x && x < max; }
	double clamp(double x) const {
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}
	static const Interval empty, universe;
};

const static Interval empty = Interval();
const static Interval universe = Interval(-infinity, +infinity);