#pragma once
#include "Hitable.h"
#include "AABB.h"

// assumes that the default rectangle is on xy plane
class rectangle : public hitable
{
public:
	rectangle(float x0, float x1, float y0, float y1, float k, material* mat);
	bool hit(const ray& ray, float t_min, float t_max, hit_record& rec) const override;
	bool bounding_box(float t0, float t1, aabb& box) const override;

protected:
	float x0, x1, y0, y1, k;
	material* mat_ptr;
};

inline rectangle::rectangle(float x0, float x1, float y0, float y1, float k, material* mat)
	: x0(x0),
      x1(x1),
      y0(y0),
      y1(y1),
      k(k),
	  mat_ptr(mat)
{

}

inline bool rectangle::hit(const ray& ray, float t_min, float t_max, hit_record& rec) const
{
	// find where it hits z axis
	float t = (k - ray.origin().z()) / ray.direction().z();
	if (t < t_min || t > t_max)
		return false;
	vec3 p = ray(t);
	if (p[0] < x0 || p[0] > x1 || p[1] < y0 || p[1] > y1)
		return false;
	rec.u = (p[0] - x0) / (x1 - x0);
	rec.v = (p[1] - y0) / (y1 - y0);
	rec.t = t;
	rec.mat_ptr = mat_ptr;
	rec.p = p;
	rec.normal = vec3(0, 0, 1);
	return true;
}

inline bool rectangle::bounding_box(float t0, float t1, aabb& box) const
{
	box = aabb(vec3(x0, y0, k - 1e-4f), vec3(x1, y1, k + 1e-4f));
	return true;
}
