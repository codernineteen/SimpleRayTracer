#pragma once
#include "Hittable.h"

class Sphere : public Hittable
{
public:
    // Stationary sphere
    Sphere(Point3 _center, double _radius, shared_ptr<Material> _material) : center1(_center), radius(_radius), mat(_material), is_moving(false) {
        auto radius_vec = Vec3(radius, radius, radius);
        bbox = AABB(center1 - radius_vec, center1 + radius_vec);
    }
    // Moving sphere
    Sphere(Point3 _center, Point3 _center2, double _radius, shared_ptr<Material> _material) : center1(_center), radius(_radius), mat(_material), is_moving(true) {
        auto radius_vec = Vec3(radius, radius, radius);
        AABB box1(center1 - radius_vec, center1 + radius_vec);
        AABB box2(_center2 - radius_vec, _center2 + radius_vec);
        bbox = AABB(box1, box2); // merge boxes.

        center_vec = _center2 - _center;
    }

    AABB bounding_box() const override { return bbox; }

    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
        Point3 center = is_moving ? sphere_center(r.get_time()) : center1;
        Vec3 oc = r.origin() - center;
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = half_b * half_b - a * c;
        if (discriminant < 0) return false;
        auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (-half_b - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (-half_b + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        Vec3 outward_normal = (rec.p - center1) / radius;
        // if front face , outward_normal itself
        // otherwise, -outward_normal
        rec.set_face_normal(r, outward_normal);
        get_sphere_uv(outward_normal, rec.u, rec.v);
        rec.mat = mat; // record material into hit record

        return true;
    }

private:
    static void get_sphere_uv(const Point3& p, double& u, double& v) {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

        auto theta = acos(-p.y());
        auto phi = atan2(-p.z(), p.x()) + pi;

        u = phi / (2 * pi);
        v = theta / pi;
    }

private:
    Point3 center1;
    double radius;
    shared_ptr<Material> mat;
    bool is_moving;
    Vec3 center_vec;
    AABB bbox;

    Point3 sphere_center(double time) const {
        return center1 + time * center_vec;
    }
};

