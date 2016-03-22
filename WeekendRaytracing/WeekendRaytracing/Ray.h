#pragma once

#include "vec3.h"

class ray
{
public:
	ray();

	ray(const vec3& a, const vec3& b, float t = 1.0f);

	vec3 origin() const;

	vec3 direction() const;

	float time() const;

	vec3 operator()(float t) const;
private:
	vec3 A;
	vec3 B;
	float _time;
};

inline ray::ray()
{
	
}

inline ray::ray(const vec3& a, const vec3& b, float t)
{
	A = a;
	B = b;
	_time = t;
}

inline vec3 ray::origin() const
{
	return A;
}

inline vec3 ray::direction() const
{
	return B;
}

inline float ray::time() const
{
	return _time;
}

inline vec3 ray::operator()(float t) const
{
	return A + B * t;
}
