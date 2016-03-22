#pragma once

#include "Vec3.h"
#include "Ray.h"

class camera
{
public:
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect);
	ray get_ray(float U, float V);

private:
	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
};

inline camera::camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect)
{
	vec3 u, v, w;
	float theta = vfov * 3.14 / 180;
	float half_height = tanf(theta / 2);
	float half_width = aspect * half_height;
	origin = lookfrom;
	w = unit_vector(lookfrom - lookat);
	u = unit_vector(cross(vup, w));
	v = cross(w, u);

	lower_left_corner = origin - half_width * u - half_height * v - w;
	horizontal = 2 * u * half_width;
	vertical = 2 * v * half_height;
}

inline ray camera::get_ray(float U, float V)
{
	return ray(origin, lower_left_corner + U * horizontal + V * vertical - origin);
}
