#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>

Color ray_color(const Ray &r)
{
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

	// Camera
	auto viewport_height = 2.0;
	auto viewport_width = aspect_ratio * viewport_height;
	auto focal_length = 1.0;

	auto origin = Point3(0, 0, 0);
	auto horizontal = Vec3(viewport_width, 0, 0);
	auto vertical = Vec3(0, viewport_height, 0);
	auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);

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
			auto u = double(i) / (image_width - 1);
			auto v = double(j) / (image_height - 1);
			Ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
			Color pixel_color = ray_color(r);
			write_color(wf, pixel_color);
		}
	}

	wf.close();
	std::cout << "\nDone!\n";
}