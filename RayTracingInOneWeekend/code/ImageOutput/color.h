#pragma once

#include "vec3.h"

#include <iostream>
#include <fstream>
#include <stdint.h>

void write_color(std::ofstream &f, Color pixel_color, int samples_per_pixel)
{
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	// divide the color by the number of samples
	auto scale = 1.0 / samples_per_pixel;
	r *= scale;
	g *= scale;
	b *= scale;

	uint8_t ur = static_cast<uint8_t>(255.999 * clamp(r, 0.0, 0.999));
	uint8_t ug = static_cast<uint8_t>(255.999 * clamp(g, 0.0, 0.999));
	uint8_t ub = static_cast<uint8_t>(255.999 * clamp(b, 0.0, 0.999));

	/*std::cout << ir << ' ' << ig << ' ' << ib << '\n';*/
	f.write((const char*)(&ur), 1);
	f.write((const char*)(&ug), 1);
	f.write((const char*)(&ub), 1);
}
