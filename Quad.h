#pragma once

#include "utilities.h"
#include "Hittable.h"
#include <cmath>

class Quad : public Hittable
{
public:
	Quad() {}
	Quad(const Point3& _Q, const Vec3& _u, const Vec3& _v, shared_ptr<Material> m)
		: Q(_Q), u(_u), v(_v), mat(m)
	{
		auto n = cross(u, v);
		normal = unit_vector(cross(u, v));
		D = dot(normal, Q);
		w = n / dot(n, n);
		set_bounding_box(); // pad if it is neccessary
	}
	~Quad() {}

	bool hit(const Ray& ray, Interval ray_t, HitRecord& rec) const {
		auto denominator = dot(normal, ray.direction());

		if (fabs(denominator) < 1e-8) { // if nearly parallel to the plane, return false
			return false;
		}

		// if intersection is out of interval, return false.
		auto t = (D - dot(normal, ray.origin())) / denominator;
		if (!ray_t.contains(t))
			return false;

		auto intersection = ray.at(t);
		Vec3 plannar_hit_point_vector = intersection - Q;
		auto alpha = dot(w, cross(plannar_hit_point_vector, v));
		auto beta = dot(w, cross(u, plannar_hit_point_vector));

		if(!is_interior(alpha, beta, rec)) 
			return false;

		rec.t = t;
		rec.p = intersection;
		rec.mat = mat;
		rec.set_face_normal(ray, normal);

		return true;
	}

	virtual bool is_interior(double a, double b, HitRecord& rec) const {
		if ((a < 0) || (1 < a) || (b < 0) || (1 < b))
			return false;

		rec.u = a;
		rec.v = b;
		return true;
	}

	void set_bounding_box() {
		bbox = AABB(Q, Q + u + v).pad();
	}

	AABB bounding_box()  const { return bbox; }

private:
	Point3 Q;
	Vec3 u, v;
	AABB bbox;
	shared_ptr<Material> mat;
	Vec3 normal;
	double D;
	Vec3 w;
};