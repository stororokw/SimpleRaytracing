#pragma once

#include "Hitable.h"
#include "Ray.h"

class moving_sphere : public hitable
{
public:
	moving_sphere();

	moving_sphere(vec3 center, vec3 center1, float t0, float t1, float radius, material* mat_ptr);
	vec3 center(float time) const;
	bool hit(const ray& ray, float & t_min, float & t_max, hit_record& rec) const override;
private:
	vec3 center0, center1;
	float radius;
	material* mat_ptr;
	float time0;
	float time1;
};

inline moving_sphere::moving_sphere()
{

}

inline moving_sphere::moving_sphere(vec3 center,vec3 center1, float t0, float t1, float radius, material* mat_ptr)
	: center0(center),
	center1(center1),
	radius(radius),
	mat_ptr(mat_ptr),
	time0(t0),
	time1(t1)
	
{}

inline vec3 moving_sphere::center(float time) const
{
	return center0 + ((time - time0) / (time1 - time0) * (center1 - center0));
}

inline bool moving_sphere::hit(const ray& r, float & t_min, float & t_max, hit_record& rec) const
{
	vec3 oc = r.origin() - center(r.time());
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
			rec.normal = unit_vector(rec.p - center(r.time()));
			rec.mat_ptr = mat_ptr;
			return true;
		}

		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r(rec.t);
			rec.normal = unit_vector(rec.p - center(r.time()));
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}

	return false;
}
