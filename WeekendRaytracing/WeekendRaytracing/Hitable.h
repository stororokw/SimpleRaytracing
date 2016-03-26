#pragma once

#include "vec3.h"
#include "Material.h"

class aabb;
class ray;

struct hit_record
{
	float t;
	vec3 p;
	vec3 normal;
	material* mat_ptr;
	float u, v;
};

class hitable
{
public:
	virtual bool hit(const ray& ray, float t_min, float t_max, hit_record& rec) const = 0;
	virtual bool bounding_box(float t0, float t1, aabb& box) const = 0;
};
