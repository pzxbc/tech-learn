#pragma once

#include "vec3.h"

#include <iostream>
#include <fstream>
#include <stdint.h>

void write_color(std::ofstream &f, Color pixel_color)
{
	uint8_t ur = static_cast<uint8_t>(255.999 * pixel_color.x());
	uint8_t ug = static_cast<uint8_t>(255.999 * pixel_color.y());
	uint8_t ub = static_cast<uint8_t>(255.999 * pixel_color.z());

	/*std::cout << ir << ' ' << ig << ' ' << ib << '\n';*/
	f.write((const char*)(&ur), 1);
	f.write((const char*)(&ug), 1);
	f.write((const char*)(&ub), 1);
}
