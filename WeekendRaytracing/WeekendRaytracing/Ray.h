#pragma once

#include "vec3.h"

class ray
{
public:
	ray();

	ray(const vec3& a, const vec3& b);

	vec3 origin() const;

	vec3 direction() const;

	vec3 operator()(float t) const;
private:
	vec3 A;
	vec3 B;
};

inline ray::ray()
{
	
}

inline ray::ray(const vec3& a, const vec3& b)
{
	A = a;
	B = b;
}

inline vec3 ray::origin() const
{
	return A;
}

inline vec3 ray::direction() const
{
	return B;
}

inline vec3 ray::operator()(float t) const
{
	return A + B * t;
}
