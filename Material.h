#pragma once

#include "utilities.h"

#include "HittableList.h"
#include "Texture.h"
#include "Color.h"

class Material 
{
public:
	virtual ~Material() = default;
	virtual bool scatter(const Ray& r, const HitRecord& rec, Color3& atteunation, Ray& scattered) const = 0;
};

class LambertianMaterial : public Material
{
public:
	LambertianMaterial(const Color3& a) : albedo(make_shared<SolidColor>(a)) {}
	LambertianMaterial(shared_ptr<Texture> a) : albedo(a) {}

	bool scatter(const Ray& r, const HitRecord& rec, Color3& atteunation, Ray& scattered) const override {
		auto scattered_direction = rec.normal + random_unit_vector();

		if(scattered_direction.near_zero()) 			
			scattered_direction = rec.normal;

		scattered = Ray(rec.p, scattered_direction, r.get_time());
		atteunation = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}

private:
	shared_ptr<Texture> albedo;
};

class MetalMaterial : public Material
{
public:
	MetalMaterial(const Color3& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	bool scatter(const Ray& r, const HitRecord& rec, Color3& atteunation, Ray& scattered) const override {
		auto reflected = reflect(unit_vector(r.direction()), rec.normal);
		scattered = Ray(rec.p, reflected + fuzz * random_unit_vector(), r.get_time());
		atteunation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

private:
	Color3 albedo;
	double fuzz;
};

class DielectricMaterial : public Material
{
public:
	DielectricMaterial(double index_of_refraction) : ir(index_of_refraction) {}

	bool scatter(const Ray& r_in, const HitRecord& rec, Color3& attenuation, Ray& scattered)
		const override {
		attenuation = Color3(1.0, 1.0, 1.0);
		double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;
		Vec3 unit_direction = unit_vector(r_in.direction());
		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
		bool cannot_refract = refraction_ratio * sin_theta > 1.0;

		Vec3 direction;
		if (cannot_refract) { // cannot refract
			direction = reflect(unit_direction, rec.normal);
		}
		else {
			direction = refract(unit_direction, rec.normal, refraction_ratio);
		}

		scattered = Ray(rec.p, direction, r_in.get_time());
		return true;
	}

private:
	double ir; // Index of Refraction

	static double reflectance(double cos, double refract_idx) {
		// Schlick's approximation for reflectance
		auto r0 = (1 - refract_idx) / (1 + refract_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cos), 5);
	}
};