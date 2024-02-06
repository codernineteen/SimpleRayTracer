#pragma once
#include "utilities.h"
#include "AABB.h"

class Material;

class HitRecord
{
public :
	Point3 p;
	Vec3 normal;
	shared_ptr<Material> mat;
	double t; // ray hit coefficient
	double u; // texture coordinate - u
	double v; // texture coordinate - v
	bool front_face;

	void set_face_normal(const Ray& ray, const Vec3& outward_normal) {
		front_face = dot(ray.direction(), outward_normal) < 0; // if the dot product is negative, ray is outside the sphere.
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class Hittable
{
public:
	virtual ~Hittable() = default;
	virtual bool hit(const Ray& ray, Interval ray_t, HitRecord& rec) const = 0;
	virtual AABB bounding_box() const = 0;
};

