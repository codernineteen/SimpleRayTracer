#pragma once

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

// usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// utility functions
inline double double_degree_to_radians(double degree) {
	return degree * pi / 180.0;
}

inline double random_double() {
	return rand() / (RAND_MAX + 1.0);
}


inline double random_double(double min, double max) {
	return min + (max-min)*random_double();
}

inline int random_int(int min, int max) {
	return static_cast<int>(random_double(min, max+1)); // [min, max+1)
}

// common Headers
#include "Interval.h"
#include "Ray.h"
#include "Vec3.h"