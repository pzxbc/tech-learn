#include "rtweekend.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>

Color ray_color(const Ray &r, const Hittable& world, int depth)
{
	HitRecord rec;

	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0)
	{
		return Color(0, 0, 0);
	}
	if (world.hit(r, 0.001, infinity, rec))
	{
		Ray scattered;
		Color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * ray_color(scattered, world, depth - 1);
		}
		return Color(0, 0, 0);
	}

	Vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main()
{
	// Image
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 100;
	const int max_depth = 50;

	// World
	HittableList world;

	auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
	auto material_center = make_shared<Dielectric>(1.5);
	auto material_left = make_shared<Dielectric>(1.5);
	auto material_right = make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

	world.add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, material_center));
	world.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

	// Camera
	Camera cam;

	std::ofstream wf("image.ppm", std::ios::out | std::ios::binary);
	if (!wf)
	{
		std::cout << "open file failed!" << std::endl;
		return 1;
	}

	//http://paulbourke.net/dataformats/ppm/
	// write header P6
	wf.write("P6\n", 3);
	auto img_width_str = std::to_string(image_width);
	wf.write(img_width_str.c_str(), img_width_str.length());
	wf.write(" ", 1);
	auto img_height_str = std::to_string(image_height);
	wf.write(img_height_str.c_str(), img_height_str.length());
	wf.write("\n", 1);
	wf.write("255\n", 4);

	for (int j = image_height - 1; j >= 0; --j)
	{
		std::cout << "\rScanlines remining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i)
		{
			Color pixel_color(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; ++s)
			{
				auto u = (i + random_double()) / (image_width - 1);
				auto v = (j + random_double()) / (image_height - 1);
				Ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);
			}
			write_color(wf, pixel_color, samples_per_pixel);
		}
	}

	wf.close();
	std::cout << "\nDone!\n";
}