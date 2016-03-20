#pragma once

#include "Vec3.h"
#include "Ray.h"

class camera
{
public:
	camera();
	ray get_ray(float U, float V);

private:
	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 veritcal;
};

inline camera::camera()
	: lower_left_corner(-2.0f, -1.0f, -1.0f),
	  horizontal(4.0f, 0.0f, 0.0f),
	  veritcal(0.0f, 2.0f, 0.0f),
	  origin(0.0f, 0.0f, 0.0f)
{

}

inline ray camera::get_ray(float U, float V)
{
	return ray(origin, lower_left_corner + U * horizontal + V * veritcal - origin);
}
