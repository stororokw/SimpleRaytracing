#pragma once

#include "Hitable.h"
#include "Ray.h"

class sphere : public hitable
{
public:
	sphere();

	sphere(vec3 center, float radius, material* mat_ptr);

	bool hit(const ray& ray, float t_min, float t_max, hit_record& rec) const override;
private:
	vec3 center;
	float radius;
	material* mat_ptr;
};

inline sphere::sphere()
{

}

inline sphere::sphere(vec3 center, float radius, material* mat_ptr)
	: center(center),
	  radius(radius),
	  mat_ptr(mat_ptr)
{}

inline bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;

	if (discriminant > 0)
	{
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}

		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}

	return false;
}
