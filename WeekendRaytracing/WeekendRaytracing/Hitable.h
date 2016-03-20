#pragma once

#include "vec3.h"
#include "Material.h"

class ray;

struct hit_record
{
	float t;
	vec3 p;
	vec3 normal;
	material* mat_ptr;
};

class hitable
{
public:
	virtual bool hit(const ray& ray, float t_min, float t_max, hit_record& rec) const = 0;
};
