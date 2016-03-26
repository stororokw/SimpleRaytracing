#pragma once

#include "Hitable.h"
#include "Ray.h"
#include "AABB.h"
#include "Mapping.h"

class sphere : public hitable
{
public:
	sphere();

	sphere(vec3 center, float radius, material* mat_ptr);

	bool hit(const ray& ray, float t_min, float t_max, hit_record& rec) const override;
	bool bounding_box(float t0, float t1, aabb& box) const override;

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

	if (discriminant > 0.0f)
	{
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r(rec.t);
			rec.normal = unit_vector(rec.p - center);
			rec.mat_ptr = mat_ptr;
			float u, v;
			get_sphere_uv(rec.normal, u, v);
			rec.u = u;
			rec.v = v;
			return true;
		}

		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r(rec.t);
			rec.normal = unit_vector(rec.p - center);
			rec.mat_ptr = mat_ptr;
			float u, v;
			get_sphere_uv(rec.normal, u, v);
			rec.u = u;
			rec.v = v;
			return true;
		}
	}

	return false;
}

inline bool sphere::bounding_box(float t0, float t1, aabb& box) const
{
	box = aabb(center - vec3(radius, radius, radius),
		center + vec3(radius, radius, radius));
	return true;
}
