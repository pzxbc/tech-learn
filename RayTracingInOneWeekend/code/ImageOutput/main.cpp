#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>

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
			auto r = double(i) / (image_width - 1);
			auto g = double(j) / (image_height - 1);
			auto b = 0.25;

			uint8_t ur = static_cast<uint8_t>(255.999 * r);
			uint8_t ug = static_cast<uint8_t>(255.999 * g);
			uint8_t ub = static_cast<uint8_t>(255.999 * b);

			/*std::cout << ir << ' ' << ig << ' ' << ib << '\n';*/
			wf.write((const char*)(&ur), 1);
			wf.write((const char*)(&ug), 1);
			wf.write((const char*)(&ub), 1);
		}
	}

	wf.close();
	std::cout << "\nDone!\n";

}