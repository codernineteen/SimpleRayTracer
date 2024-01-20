#pragma once

#include <fstream>
#include "Vec3.h"

using Color3 = Vec3;

// add inline keyword to avoid duplicate symbol error
inline void write_color(std::ofstream& out, Color3 pixel_color) {
		out << static_cast<int>(255.999 * pixel_color.x()) << ' '
		<< static_cast<int>(255.999 * pixel_color.y()) << ' '
		<< static_cast<int>(255.999 * pixel_color.z()) << '\n';
}