#pragma once

#include "Vec3.h"

#include <fstream>

using Color3 = Vec3;

// gamma correction
// an image viewer may assume our image in gamma space, but actually our image is in linear space
// so we need to convert it to gamma space in advance as expected.
inline double linear_to_gamma(double x) {
	return sqrt(x);
}

// add inline keyword to avoid duplicate symbol error
inline void write_color(std::ofstream& out, Color3 pixel_color, int samples_per_pixel) {
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	auto scale = 1.0 / samples_per_pixel;
	r *= scale;
	g *= scale;
	b *= scale;

	r = linear_to_gamma(r);
	g = linear_to_gamma(g);
	b = linear_to_gamma(b);

	static const Interval intensity(0.000, 0.999);
	out << static_cast<int>(255.999 * intensity.clamp(r)) << ' '
	<< static_cast<int>(255.999 * intensity.clamp(g)) << ' '
	<< static_cast<int>(255.999 * intensity.clamp(b)) << '\n';
}