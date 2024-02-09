#include "utilities.h"

#include "Color.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"
#include "BVH.h"
#include "Texture.h"
#include "Quad.h"

#include <iostream>
#include <fstream>

void earth() {
    auto earth_texture = make_shared<ImageTexture>("earthmap.jpg");
    auto earth_surface = make_shared<LambertianMaterial>(earth_texture);
    auto globe = make_shared<Sphere>(Point3(0, 0, 0), 2, earth_surface);

    Camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.fov = 20;
    cam.lookfrom = Point3(0, 0, 12);
    cam.lookat = Point3(0, 0, 0);
    cam.vup = Vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(HittableList(globe));
}

void random_spheres() {
    // World
    HittableList world;

    auto ground_material = make_shared<LambertianMaterial>(Color3(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

    for (int a = -3; a < 3; a++) {
        for (int b = -3; b < 3; b++) {
            auto choose_mat = random_double();
            Point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color3::random() * Color3::random();
                    sphere_material = make_shared<LambertianMaterial>(albedo);  
                    auto center2 = center + Vec3(0, random_double(0, 0.5), 0);
                    world.add(make_shared<Sphere>(center, center2, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color3 ::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<MetalMaterial>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<DielectricMaterial>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<DielectricMaterial>(1.5);
    world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<LambertianMaterial>(Color3(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<MetalMaterial>(Color3(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));


    world = HittableList(make_shared<BVHNode>(world));
    // Camera
    Camera cam;
    
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.fov = 20;
    cam.lookfrom = Point3(13, 2, 3);
    cam.lookat = Point3(0, 0, 0);
    cam.vup = Vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    // Render
    cam.render(world);
}

void two_spheres() {
// World
	HittableList world;

    auto checker = make_shared<CheckerTexture>(0.8, Color3(.2, .3, .1), Color3(.9, .9, .9));

	world.add(make_shared<Sphere>(Point3(0, -10, 0), 10, make_shared<LambertianMaterial>(checker)));
	world.add(make_shared<Sphere>(Point3(0, 10, 0), 10, make_shared<LambertianMaterial>(checker)));

	// Camera
	Camera cam;
	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;

	cam.fov = 20;
	cam.lookfrom = Point3(13, 2, 3);
	cam.lookat = Point3(0, 0, 0);
	cam.vup = Vec3(0, 1, 0);

    cam.defocus_angle = 0;

	// Render
	cam.render(world);
}

void two_perlin_spheres() {
    HittableList world;

    auto pertext = make_shared<NoiseTexture>(4);
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<LambertianMaterial>(pertext)));
    world.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<LambertianMaterial>(pertext)));

    Camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.fov = 20;
    cam.lookfrom = Point3(13, 2, 3);
    cam.lookat = Point3(0, 0, 0);
    cam.vup = Vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void quads() {
    HittableList world;

    // Materials
    auto left_red = make_shared<LambertianMaterial>(Color3(1.0, 0.2, 0.2));
    auto back_green = make_shared<LambertianMaterial>(Color3(0.2, 1.0, 0.2));
    auto right_blue = make_shared<LambertianMaterial>(Color3(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<LambertianMaterial>(Color3(1.0, 0.5, 0.0));
    auto lower_teal = make_shared<LambertianMaterial>(Color3(0.2, 0.8, 0.8));

    // Quads
    world.add(make_shared<Quad>(Point3(-3, -2, 5), Vec3(0, 0, -4),Vec3(0, 4, 0), left_red));
    world.add(make_shared<Quad>(Point3(-2, -2, 0), Vec3(4, 0, 0), Vec3(0, 4, 0), back_green));
    world.add(make_shared<Quad>(Point3(3, -2, 1),  Vec3(0, 0, 4), Vec3(0, 4, 0), right_blue));
    world.add(make_shared<Quad>(Point3(-2, 3, 1),  Vec3(4, 0, 0), Vec3(0, 0, 4), upper_orange));
    world.add(make_shared<Quad>(Point3(-2, -3, 5), Vec3(4, 0, 0), Vec3(0, 0, -4), lower_teal));

    Camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.fov = 80;
    cam.lookfrom = Point3(0, 0, 9);
    cam.lookat = Point3(0, 0, 0);
    cam.vup = Vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world);
}

int main() {
    quads();
}