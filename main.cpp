#include "Vec3.h"
#include "Color.h"
#include "Ray.h"
#include <iostream>
#include <fstream>

bool hitSphere(const Point3& p, double radius, const Ray& r) {
	Vec3 oc = r.origin() - p;
	auto a = dot(r.direction(), r.direction());
	auto b = 2.0 * dot(r.direction(), oc);
	auto c = dot(oc, oc) - radius * radius;
	auto discriminant = b * b - 4 * a * c;
	return (discriminant >= 0);
}

Color3 rayColor(const Ray& r) {
	if (hitSphere(Point3(0, 0, -1), 0.5, r))
		return Color3(1, 0, 0);

	Vec3 unitDirection = unit_vector(r.direction());
	auto a = 0.5 * (unitDirection.y() + 1.0); // map y from [-1, 1] to [0, 1]
	return (1.0 - a) * Color3(1.0, 1.0, 1.0) + a * Color3(0.5, 0.7, 1.0); // linear interpolation
}

int main() {
	auto aspectRatio = 16.0 / 9.0;
	int imageWidth = 400;
	int imageHeight = static_cast<int>(imageWidth / aspectRatio);
	imageHeight = (imageHeight < 1) ? 1 : imageHeight;

	// camera
	auto focalLength = 1.0;
	auto viewportHeight = 2.0;
	auto viewportWidth = viewportHeight * (static_cast<double>(imageWidth) / imageHeight);
	auto cameraOrigin = Point3(0, 0, 0);

	// viewport edge vectors
	auto viewportUVec = Vec3(viewportWidth, 0, 0);
	auto viewportVVec = Vec3(0, -viewportHeight, 0);
	// pixel delta between two pixels
	auto pixelUVec = viewportUVec / imageWidth;
	auto pixelVVec = viewportVVec / imageHeight;
	// location of upper left pixel
	auto viewportUpperLeft = cameraOrigin - Vec3(0, 0, focalLength) - viewportUVec/2 + viewportVVec/2;
	auto pixelOrigin = viewportUpperLeft + 0.5 * (pixelUVec + pixelVVec);


	std::ofstream outputFile("output.ppm");
	outputFile << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

	for (int j = 0; j < imageHeight; ++j) {
		std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
		for (int i = 0; i < imageWidth; ++i) {
			auto pixel_center = pixelOrigin + (i * pixelUVec) + (j * pixelVVec);
			auto ray_direction = pixel_center - cameraOrigin;
			Ray r(cameraOrigin, ray_direction);

			Color3 pixel_color = rayColor(r);
			write_color(outputFile, pixel_color);
		}
	}

	std::clog << "\rGenerated an image.                \n";
}