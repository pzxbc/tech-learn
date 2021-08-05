#pragma once

#include "rtweekend.h"

struct HitRecord;

class Material {
public:
	virtual bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const = 0;
};

class Lambertian : public Material {
public:
	Lambertian(const Color &a) : albedo(a) {}

	virtual bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override
	{
		auto scatter_direction = rec.normal + random_unit_vector();

		// Catch degenerate scatter direction
		if (scatter_direction.near_zero())
		{
			scatter_direction = rec.normal;
		}

		scattered = Ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}

public:
	Color albedo;
};

class Metal : public Material {
public:
	Metal(const Color &a, double f) : albedo(a), fuzz(f<1? f: 1) {}

	virtual bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override
	{
		Vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = Ray(rec.p, reflected + fuzz*random_in_unit_sphere());
		attenuation = albedo;

		return (dot(scattered.direction(), rec.normal) > 0);
	}
public:
	Color albedo;
	double fuzz;
};

class Dielectric : public Material {
public:
	Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

	virtual bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override
	{
		attenuation = Color(1.0, 1.0, 1.0);
		double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

		Vec3 unit_direction = unit_vector(r_in.direction());
		Vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);

		scattered = Ray(rec.p, refracted);
		return true;
	}

public:
	double ir;
};