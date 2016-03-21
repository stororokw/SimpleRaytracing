#include <iostream>
#include <fstream>
#include "Vec3.h"
#include "Ray.h"
#include "Hitable.h"
#include "Sphere.h"
#include "HitableList.h"
#include "RNG.h"
#include "Camera.h"
#include "lambertian.h"
#include "metal.h"
#include "Dielectric.h"

vec3 colour(const ray& r, hitable* world, int depth)
{
	hit_record rec;
	if (world->hit(r, 1e-6, INFINITY, rec))
	{
		ray scattered_ray;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered_ray))
		{
			return attenuation * colour(scattered_ray, world, depth + 1);
		} 
		return vec3(0, 0, 0);
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

	hitable* geometry[4];
	geometry[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)));
	geometry[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	geometry[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
	geometry[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1));

	hitable* world = new hitable_list(geometry, 4);
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
				c+= colour(r, world, 0);
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