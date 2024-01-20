#pragma once
#include "Ray.h"

class HitRecord
{
public :
	Point3 p;
	Vec3 normal;
	double t;
};

class Hittable
{
public:
	virtual ~Hittable() = default;
	virtual bool hit(const Ray& ray, double ray_tmin, double ray_tmax, HitRecord& rec) const = 0;
};

