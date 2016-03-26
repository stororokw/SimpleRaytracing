#pragma once

#include "Vec3.h"
#define _USE_MATH_DEFINES
#include <math.h>

inline void get_sphere_uv(const vec3& p, float& u, float& v)
{
	float phi = atan2f(p.z(), p.x());
	float theta = asinf(p.y());
	u = 1 - (phi + M_PI) / (2 * M_PI);
	v = (theta + M_PI) / M_PI;
}
