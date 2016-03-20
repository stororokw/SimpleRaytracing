#include <iostream>
#include <fstream>
#include "Vec3.h"
#include "Ray.h"
#include "Hitable.h"
#include "Sphere.h"
#include "HitableList.h"
#include "RNG.h"
#include "Camera.h"

vec3 random_in_unit_sphere()
{
	vec3 p;
	do
	{
		// make sure point in unit cube is in the range of -1 to 1
		p = 2.0f * vec3(RNG::random(), RNG::random(), RNG::random()) - vec3(1, 1, 1);
	} while (dot(p, p) >= 1.0f);
	return p;
}

vec3 colour(const ray& r, hitable* world)
{
	hit_record rec;
	if (world->hit(r, 0.0f, INFINITY, rec))
	{
		vec3 s = rec.p + rec.normal + random_in_unit_sphere();
		return 0.5 * colour(ray(rec.p, s - rec.p), world);
	}
	vec3 unit_direction = unit_vector(r.direction());
	// map to the range 0-1
	float t = 0.5 * (unit_direction.y() + 1.0);
	vec3 white = vec3(1, 1, 1);
	vec3 blue = vec3(0.5, 0.7, 1.0);
	return lerp(t, white, blue);
}

int main()
{
	int nx = 200;
	int ny = 100;
	int ns = 100;
	std::ofstream output("image.ppm");

	vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
	vec3 horizontal(4.0f, 0.0f, 0.0f);
	vec3 vertical(0.0f, 2.0f, 0.0f);
	vec3 origin(0.0f, 0.0f, 0.0f);

	hitable* geometry[2];
	geometry[0] = new sphere(vec3(0, 0, -1), 0.5);
	geometry[1] = new sphere(vec3(0, -100.5, -1), 100);

	hitable* world = new hitable_list(geometry, 2);
	camera cam;
	output << "P3\n" << nx << " " << ny << "\n255\n";
	for (int row = ny - 1; row >= 0; --row)
	{
		for (int col = 0; col < nx; ++col)
		{
			vec3 c(0.0f, 0.0f, 0.0f);
			for (int s = 0; s < ns; ++s)
			{
				float u = float(col + RNG::random()) / float(nx);
				float v = float(row + RNG::random()) / float(ny);
				ray r = cam.get_ray(u, v);
				c+= colour(r, world);
			}
			c /= float(ns);
			// gamma correct
			c = vec3(sqrtf(c[0]), sqrtf(c[1]), sqrtf(c[2]));
			int ir = int(255.99 * c[0]);
			int ig = int(255.99 * c[1]);
			int ib = int(255.99 * c[2]);
			output << ir << " " << ig << " " << ib << "\n";
		}
	}
	output.close();

	return 0;
}