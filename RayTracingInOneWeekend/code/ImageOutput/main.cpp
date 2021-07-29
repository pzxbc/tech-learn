#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>

#include "color.h"
#include "vec3.h"

int main() {
	// Image

	const int image_width = 256;
	const int image_height = 256;

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
			Color pixel_color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0.25);
			write_color(wf, pixel_color);
		}
	}

	wf.close();
	std::cout << "\nDone!\n";

}