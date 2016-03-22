#pragma once

#include "Vec3.h"
#include "Ray.h"

class camera
{
public:
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect,float aperture, float focus_distance);
	ray get_ray(float U, float V);

private:
	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	float lens_radius;
};

inline camera::camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_distance)
{
	lens_radius = aperture / 2;
	vec3 u, v, w;
	float theta = vfov * 3.14 / 180;
	float half_height = tanf(theta / 2);
	float half_width = aspect * half_height;
	origin = lookfrom;
	w = unit_vector(lookfrom - lookat);
	u = unit_vector(cross(vup, w));
	v = cross(w, u);

	lower_left_corner = origin - half_width * u * focus_distance - half_height * v * focus_distance - focus_distance * w;
	horizontal = 2 * u * half_width * focus_distance;
	vertical = 2 * v * half_height * focus_distance;
}

inline ray camera::get_ray(float U, float V)
{
	vec3 rd = lens_radius * RNG::random_in_unit_disc();
	vec3 offset = vec3(U * rd.x(), V * rd.y(), 0);
	return ray(origin + offset, unit_vector(lower_left_corner + U * horizontal + V * vertical - origin - offset));
}
