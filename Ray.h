#pragma once
#include "Vec3.h"

class Ray
{
private:
	Point3 orig;
	Vec3 dir;
	double time;

public:
	Ray() {}
	Ray(const Point3& origin, const Vec3& direction) : orig(origin), dir(direction), time(0.0) {}
	Ray(const Point3& origin, const Vec3& direction, double time = 0.0) : orig(origin), dir(direction), time(time) {}

	Point3 origin() const { return orig; }
	Vec3 direction() const { return dir; }
	double get_time() const { return time; }

	Point3 at(double t) const {
		return orig + t * dir;
	}
};

