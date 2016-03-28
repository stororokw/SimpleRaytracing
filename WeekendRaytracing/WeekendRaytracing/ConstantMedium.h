#pragma once
#include "Hitable.h"
#include "isotropic.h"

class texture;

class constant_medium : public hitable
{
public:
	constant_medium(hitable* boundary, float density, texture* a);

	bool bounding_box(float t0, float t1, aabb& box) const override;
	bool hit(const ray& ray, float& t_min, float& t_max, hit_record& rec) const override;

protected:
	hitable* boundary;
	float density;
	material* phase_function;
};

inline constant_medium::constant_medium(hitable* boundary, float density, texture* a)
	: boundary(boundary), density(density), phase_function(new isotropic(a))
{

}

inline bool constant_medium::bounding_box(float t0, float t1, aabb& box) const
{
	return boundary->bounding_box(t0, t1, box);
}

inline bool constant_medium::hit(const ray& ray, float& t_min, float& t_max, hit_record& rec) const
{
	bool db = RNG::random() < 1e-5f;
	db = false;
	hit_record rec1, rec2;
	float _min = -FLT_MAX, _max = FLT_MAX;
	float step = 0.0001;
	if(boundary->hit(ray, _min, _max, rec1))
	{
		_min = rec1.t + step;
		_max = FLT_MAX;
		if (boundary->hit(ray,_min , _max, rec2))
		{
			if (rec1.t < t_min)
				rec1.t = t_min;
			if (rec2.t > t_max)
				rec2.t = t_max;
			if (rec1.t >= rec2.t)
				return false;
			if (rec1.t < 0.0f)
				rec1.t = 0;
			float distance_inside_boundary = (rec2.t - rec1.t) * ray.direction().length();
			float hit_distance = -(1 / density) * log(RNG::random());
			if (hit_distance < distance_inside_boundary)
			{
				rec.t = rec1.t + hit_distance / ray.direction().length();
				rec.p = ray(rec.t);
				rec.normal = vec3(1, 0, 0);
				rec.mat_ptr = phase_function;
				return true;
			}
		}
	}
	return false;
}
