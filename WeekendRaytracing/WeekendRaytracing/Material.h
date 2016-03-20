#pragma once

#include "Vec3.h"

struct hit_record;
class ray;

class material
{
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
	static vec3 reflect(const vec3& v, const vec3& n);
};

inline vec3 material::reflect(const vec3& v, const vec3& n)
{
	return v - 2 * dot(v, n) * n;
}