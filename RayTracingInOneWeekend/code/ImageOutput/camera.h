#pragma once

#include "rtweekend.h"

class Camera {
public:
	Camera(double vfov, double aspect_ratio) {
		auto theta = degrees_to_radians(vfov);
		auto h = tan(theta / 2);
		auto viewport_height = 2.0 * h;
		auto viewport_width = aspect_ratio * viewport_height;
		auto focal_length = 1.0;

		origin = Point3(0, 0, 0);
		horizontal = Vec3(viewport_width, 0.0, 0.0);
		vertical = Vec3(0.0, viewport_height, 0.0);
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);
	}

	Ray get_ray(double u, double v) const {
		return Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
	}

private:
	Point3 origin;
	Point3 lower_left_corner;
	Vec3 horizontal;
	Vec3 vertical;
};