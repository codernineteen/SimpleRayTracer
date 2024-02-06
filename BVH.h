#pragma once

#include "utilities.h"

#include "Hittable.h"
#include "HittableList.h"

#include <algorithm>

class BVHNode : public Hittable {
	
private:
	shared_ptr<Hittable> left;
	shared_ptr<Hittable> right;
	AABB bbox;

public :
	BVHNode(const HittableList& list) : BVHNode(list.objects, 0, list.objects.size()) {}
	BVHNode(const std::vector<shared_ptr<Hittable>>& src_objects, size_t start, size_t end) {
		auto objects = src_objects;

		int axis = random_int(0, 2);
		auto comparator = (axis == 0) ? box_x_compare
						: (axis == 1) ? box_y_compare
									  : box_z_compare;

		size_t object_span = end - start;
		if (object_span == 1) {
			left = right = objects[start];
		}
		else if (object_span == 2) {
			if(comparator(objects[start], objects[start+1])) {
				left = objects[start];
				right = objects[start + 1];
			}
			else {
				left = objects[start + 1];
				right = objects[start];
			}
		}
		else {
			std::sort(objects.begin(), objects.end(), comparator); // sort by minimum boundary of bounding boxes

			auto mid = start + object_span / 2;
			left = make_shared<BVHNode>(objects, start, mid);
			right = make_shared<BVHNode>(objects, mid, end);
		}

		bbox = AABB(left->bounding_box(), right->bounding_box());
	}

	bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
		if (!bbox.hit(r, ray_t))
			return false;

		// sub-volume hit test.
		bool hit_left = left->hit(r, ray_t, rec);
		bool hit_right = right->hit(r, Interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

		return hit_left || hit_right;
	}

	AABB bounding_box() const override {
		return bbox;
	}
private:
	static bool box_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axis_index) {
		return a->bounding_box().axis(axis_index).min < b->bounding_box().axis(axis_index).min;
	}

	static bool box_x_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
		return box_compare(a, b, 0);
	}

	static bool box_y_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
		return box_compare(a, b, 1);
	}

	static bool box_z_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
		return box_compare(a, b, 2);
	}
};