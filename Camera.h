#pragma once
#include "utilities.h"

#include "Color.h"
#include "Hittable.h"
#include "Material.h"

#include <fstream>
#include <iostream>

/*
	Camera class
		- Construct and dispatch rays into the world
		- Use the result of these rays to construct the rendered image.
*/
class Camera
{
public:
	double aspect_ratio = 1.0;
	int image_width = 100;
	int samples_per_pixel = 10;
	int max_depth = 10;

	double fov = 90.0;
	Point3 lookfrom = Point3(0, 0, -1);
	Point3 lookat = Point3(0, 0, 0);
	Vec3 vup = Vec3(0, 1, 0);

	double defocus_angle = 0;
	double focus_dist = 10;

	void render(const Hittable& world) {
		initialize();

		std::ofstream outputFile("output.ppm");
		outputFile << "P3\n" << image_width << " " << image_height << "\n255\n";

		for (int j = 0; j < image_height; ++j) {
			std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
			for (int i = 0; i < image_width; ++i) {
				Color3 pixel_color(0, 0, 0);
				for (int sample = 0; sample < samples_per_pixel; sample++) {
					Ray r = get_ray(i, j);
					pixel_color += ray_color(r, max_depth, world);
				}
				write_color(outputFile, pixel_color, samples_per_pixel);
			}
		}
		std::clog << "\rDone                 "<< std::flush;
	}

private:
	int image_height;
	Point3 center;
	Point3 pixel00_loc;
	Vec3 pixel_delta_u;
	Vec3 pixel_delta_v;
	Vec3 u, v, w;
	Vec3 defocus_disk_u;
	Vec3 defocus_disk_v;

	void initialize() {
		image_height = static_cast<int>(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;

		center = lookfrom;

		// Determine viewport dimensions.
		auto theta = double_degree_to_radians(fov);
		auto h = tan(theta / 2); // tangent against half of  fov
		auto viewport_height = 2 * h * focus_dist;
		auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
		// calculate orthonormal basis for camera
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		// Calculate the vectors across the horizontal and down the vertical viewport edges.
		Vec3 viewport_u = viewport_width * u;
		Vec3 viewport_v = viewport_height * -v;

		// Calculate the horizontal and vertical delta vectors from pixel to pixel.
		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		// Calculate the location of the upper left pixel.
		auto viewport_upper_left =
			center - (focus_dist*w)- viewport_u/2 - viewport_v/2;
		pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

		auto defocus_radius = focus_dist * tan(double_degree_to_radians(defocus_angle / 2));
		defocus_disk_u = u * defocus_radius;
		defocus_disk_v = v * defocus_radius;
	}

	Color3 ray_color(const Ray& r, int depth, const Hittable& world) const {
		if (depth <= 0)
			return Color3(0, 0, 0);

		HitRecord rec;
		if (world.hit(r, Interval(0.001, infinity), rec)) {
			Ray scattered;
			Color3 atteunation;
			if (rec.mat->scatter(r, rec, atteunation, scattered))
				return atteunation * ray_color(scattered, depth - 1, world);
			return Color3(0, 0, 0);
		}
		Vec3 unit_direction = unit_vector(r.direction());
		auto a = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - a) * Color3(1.0, 1.0, 1.0) + a * Color3(0.5, 0.7, 1.0);
	}

	Ray get_ray(int i, int j) const {
		auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
		auto pixel_sample = pixel_center + pixel_sample_square();

		auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
		auto ray_direction = pixel_sample - center;
		auto ray_time = random_double();

		return Ray(center, ray_direction, ray_time);
	}

	Point3 defocus_disk_sample() const {
		auto p = random_in_unit_disk();
		return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
	}

	Vec3 pixel_sample_square() const {
		auto px = -0.5 + random_double();
		auto py = -0.5 + random_double();
		return px * pixel_delta_u + py * pixel_delta_v;
	}
};

