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
#include "MovingSphere.h"
#include "BVH.h"

hitable** random_scene(int& size)
{
	int n = 500;
	hitable** list = new hitable*[n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; ++a)
	{
		for (int b = -11; b < 11; ++b)
		{
			float choose_mat = RNG::random();
			vec3 center(a + 0.9 * RNG::random(), 0.2, b + 0.9 * RNG::random());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9)
			{
				// diffuse
				if (choose_mat < 0.8)
				{
					list[i++] = new sphere(center, 0.2, new lambertian(vec3(RNG::random() * RNG::random(), RNG::random() * RNG::random(), RNG::random() * RNG::random())));
				}
				else if (choose_mat < 0.95)
				{
					// metal
					list[i++] = new sphere(center, 0.2, new metal(vec3(0.5 * (1 + RNG::random()),
																  0.5 * (1 + RNG::random()),
																  0.5 * (1 + RNG::random())), 0.5 * RNG::random()));
				}
				else
				{
					// glass
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0));
	size = i;
	return list;
}

vec3 colour(const ray& r, hitable* world, int depth)
{
	hit_record rec;
	if (world->hit(r, 1e-5f, INFINITY, rec))
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

	int size;
	hitable** scene = random_scene(size);
	hitable* world = new bvh_node(scene, size, 0, 1e11);
	//camera cam(vec3(-2,2,1),vec3(0,0,-1),vec3(0,1,0), 15, float(nx)/ ny);
	camera cam(vec3(13, 2, 3), vec3(0, 0, 0), vec3(0, 1, 0), 20, float(nx) / ny, 0, 10, 0, 1);
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
			c = vec3(sqrtf(c[0]), sqrtf(c[1]), sqrtf(c[2])).clamp();
			int ir = int(255.99 * c[0]);
			int ig = int(255.99 * c[1]);
			int ib = int(255.99 * c[2]);
			output << ir << " " << ig << " " << ib << "\n";
		}
	}
	output.close();

	return 0;
}