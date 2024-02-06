#pragma once

#include "Hittable.h"
#include "AABB.h"

#include <vector>
#include <memory>

/*
	HittableList 
	- a list to store generic hittable objects
*/
class HittableList : public Hittable
{
public:
	std::vector<std::shared_ptr<Hittable>> objects;

	HittableList() {}
	HittableList(std::shared_ptr<Hittable> object) { add(object); }

	void clear() { objects.clear(); }
	void add(std::shared_ptr<Hittable> object) { 
		objects.push_back(object); 
		bbox = AABB(bbox, object->bounding_box()); // expand box whenever there is a new object
	}

	bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
		HitRecord temp_rec;
		bool hit_anything = false;
		auto closest_so_far = ray_t.max;

		// Store the closest hit object about this ray
		for (const auto& object : objects) {
			if (object->hit(r, Interval(ray_t.min, closest_so_far), temp_rec)) {
				hit_anything = true;
				closest_so_far = temp_rec.t;
				rec = temp_rec;
			}
		}

		return hit_anything;
	}

	// bounding box getter
	AABB bounding_box() const override { return bbox; }

private:
	AABB bbox;
};

