#pragma once

#include "utilities.h"

class AABB {
public:
	Interval x, y, z;

	AABB() {};
	AABB(const Interval& ix, const Interval& iy, const Interval& iz) : x(ix), y(iy), z(iz) {}
	AABB(const Point3& a, const Point3& b) {
		x = Interval(fmin(a[0], b[0]), fmax(a[0], b[0]));
		y = Interval(fmin(a[1], b[1]), fmax(a[1], b[1]));
		z = Interval(fmin(a[2], b[2]), fmax(a[2], b[2]));
	};
	AABB(const AABB& box1, const AABB& box2) {
		x = Interval(box1.x, box2.x);
		y = Interval(box1.y, box2.y);
		z = Interval(box1.z, box2.z);
	}

	const Interval& axis(int n) const {
		if (n == 1) return y;
		if (n == 2) return z;
		return x;
	}

	bool hit(const Ray& r, Interval ray_t) const {
		for (int a = 0; a < 3; a++) {
			auto invD = 1.0 / r.direction()[a]; // inverted direction
			auto orig = r.origin()[a];

			auto t0 = (axis(a).min - orig) * invD;
			auto t1 = (axis(a).max - orig) * invD;

			// check whether direction is negative. if so, swap the t0 and t1.
			if (invD < 0.0) std::swap(t0, t1);

			if(t0 > ray_t.min) ray_t.min = t0;
			if (t1 < ray_t.max) ray_t.max = t1;
			
			if (ray_t.max <= ray_t.min) return false;
		}
		return true;
	}

	AABB pad() {
		double delta = 0.0001; // padding
		Interval new_x = (x.size() >= delta) ? x : x.expand(delta);
		Interval new_y = (y.size() >= delta) ? y : y.expand(delta);
		Interval new_z = (z.size() >= delta) ? z : z.expand(delta);

		return AABB(new_x, new_y, new_z);
	}
};