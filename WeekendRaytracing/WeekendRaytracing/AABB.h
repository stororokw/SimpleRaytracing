#pragma once

#include "Vec3.h"
#include "Ray.h"
#include <algorithm>

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class aabb
{
public:
	aabb();
	aabb(const vec3& a, const vec3& b);

	vec3 min() const;
	vec3 max() const;

	bool hit(const ray& r, float tmin, float tmax) const;
	int longest_axis();
	float area();
	aabb operator+(const vec3& p) const;
	// returns the union of the two boxes
	static aabb surrounding_box(const aabb& box0, const aabb& box1);

	vec3 _min;
	vec3 _max;
};

inline aabb::aabb()
{

}

inline aabb::aabb(const vec3& a, const vec3& b)
	: _min(a), _max(b)
{

}

inline vec3 aabb::min() const
{
	return _min;
}

inline vec3 aabb::max() const
{
	return _max;
}

inline bool aabb::hit(const ray& r, float tmin, float tmax) const
{
	for (int a = 0; a < 3; ++a)
	{
		float invD = 1.0f / r.direction()[a];
		// compute intersections with slabs
		float t0 = (min()[a] - r.origin()[a]) * invD;
		float t1 = (max()[a] - r.origin()[a]) * invD;
		// ray is in opposite direction
		if (invD < 0.0f)
		{
			std::swap(t0, t1);
		}
		// check intervals
		tmin = t0 > tmin ? t0 : tmin;
		tmax = t1 < tmax ? t1 : tmax;
		if (tmax <= tmin)
			return false;
	}
	return true;
}

inline int aabb::longest_axis()
{
	vec3 axis = _max - _min;
	float maxAxisValue = -INFINITY;
	int maxIndex = 0;
	for (int i = 0; i < 3; ++i)
	{
		if (fabsf(axis[i]) > maxAxisValue)
		{
			maxAxisValue = fabsf(axis[i]);
			maxIndex = i;
		}
	}
	return maxIndex;
}

inline float aabb::area()
{
	float x = _max[0] - _min[0];
	float y = _max[1] - _min[1];
	float z = _max[2] - _min[2];
	return 2 * (x * y + x * z + z * y);
}

inline aabb aabb::operator+(const vec3& p) const
{
	aabb result;
	result._min[0] = (fminf(p[0], min()[0]));
	result._min[1] = (fminf(p[1], min()[1]));
	result._min[2] = (fminf(p[2], min()[2]));
	result._max[0] = (fmaxf(p[0], max()[0]));
	result._max[1] = (fmaxf(p[1], max()[1]));
	result._max[2] = (fmaxf(p[2], max()[2]));

	return result;
}

inline aabb aabb::surrounding_box(const aabb& box0, const aabb& box1)
{
	vec3 small( fminf(box0.min()[0], box1.min()[0]),
	            fminf(box0.min()[1], box1.min()[1]),
	            fminf(box0.min()[2], box1.min()[2]));
	vec3 large( fmaxf(box0.max()[0], box1.max()[0]),
	            fmaxf(box0.max()[1], box1.max()[1]),
	            fmaxf(box0.max()[2], box1.max()[2]));
	return aabb(small, large);
}
