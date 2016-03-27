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
#include "Bitmap.h"
#include "omp.h"
#include "Timer.h"
#include "ConstantTexture.h"
#include "CheckerTexture.h"
#include "ImageTexture.h"
#include "EmitterMaterial.h"
#include "Rectangle.h"
#include "Mesh.h"

hitable** simple_light(int& size)
{
	texture* green = new constant_texture(vec3(0.2, 0.3, 0.1));
	texture* gray = new constant_texture(vec3(0.9, 0.9, 0.9));

	texture* checker = new checker_texture(green, gray);
	hitable** l = new hitable*[4];
	l[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(checker));
	l[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(checker));
	l[2] = new sphere(vec3(0, 7, 0), 2, new emitter_material(new constant_texture(vec3(4,4,4))));
	l[3] = new rectangle(3, 5, 1, 3, -2, new emitter_material(new constant_texture(vec3(4, 4, 4))));
	size = 4;
	return l;
}

hitable** two_spheres(int& size)
{
	texture* green = new constant_texture(vec3(0.2, 0.3, 0.1));
	texture* gray = new constant_texture(vec3(0.9, 0.9, 0.9));

	texture* checker = new checker_texture(green, gray);
	int n = 2;
	hitable** list = new hitable*[n];
	list[0] = new sphere(vec3(0, -10, 0), 10, new lambertian(checker));
	list[1] = new sphere(vec3(0,  10, 0), 10, new lambertian(checker));
	size = n;
	return list;
}

hitable** random_scene(int& size)
{
	int n = 500;
	hitable** list = new hitable*[n + 1];
	texture* gray = new constant_texture(vec3(0.9, 0.9, 0.9));
	texture* green = new constant_texture(vec3(0.2, 0.3, 0.1));
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(new checker_texture(green, gray)));
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
					texture* r = new constant_texture(vec3(RNG::random() * RNG::random(), RNG::random() * RNG::random(), RNG::random() * RNG::random()));
					list[i++] = new sphere(center, 0.2, new lambertian(r));
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

	//texture* d = new constant_texture(vec3(0.4, 0.2, 0.1));
	Bitmap* b = Bitmap::LoadPPM6(R"(texture.ppm)");
	texture* d = new image_texture(b);
	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(d));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0));
	size = i;
	return list;
}

vec3 colour(const ray& r, hitable* world, int depth)
{
	hit_record rec;
	float t_min = 1e-4f;
	float t_max = INFINITY;
	if (world->hit(r, t_min, t_max, rec))
	{
		ray scattered_ray;
		vec3 attenuation;
		vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
		if (depth < 10 && rec.mat_ptr->scatter(r, rec, attenuation, scattered_ray))
		{
			return emitted + attenuation * colour(scattered_ray, world, depth + 1);
		} 
		else
		{
			return emitted;
		}
	}
	return vec3(0, 0, 0);
	//vec3 unit_direction = unit_vector(r.direction());
	//// map to the range 0-1
	//float t = 0.5 * (unit_direction.y() + 1.0);
	//vec3 white = vec3(1, 1, 1);
	//vec3 blue = vec3(0.5, 0.7, 1.0);
	//return lerp(t, white, blue);
}

int main()
{
	Timer timer;
	int nx = 512;
	int ny = 512;
	int ns = 512;
	Bitmap bitmap(nx, ny);
	int size;
	//hitable** scene = random_scene(size);
	//hitable** scene = two_spheres(size);
	hitable** scene = simple_light(size);
	mesh* left_wall = mesh::LoadMesh("left_wall.obj", new lambertian(new constant_texture(vec3(150 / 255.0, 14 / 255.0, 0))));
	mesh* right_wall = mesh::LoadMesh("right_wall.obj", new lambertian(new constant_texture(vec3(38 / 255.0, 101 / 255.0, 21 / 255.0))));
	mesh* bottom_floor = mesh::LoadMesh("bottom_floor.obj", new lambertian(new constant_texture(vec3(1, 1, 1))));
	mesh* top_ceiling = mesh::LoadMesh("top_ceiling.obj", new lambertian(new constant_texture(vec3(1, 1, 1))));
	mesh* small_box = mesh::LoadMesh("small_box.obj", new lambertian(new constant_texture(vec3(1, 1, 1))));
	mesh* large_box = mesh::LoadMesh("large_box.obj", new lambertian(new constant_texture(vec3(1, 1, 1))));
	mesh* back_wall = mesh::LoadMesh("back_wall.obj", new lambertian(new constant_texture(vec3(1, 1, 1))));
	mesh* light = mesh::LoadMesh("light.obj", new emitter_material(new constant_texture(9 * vec3(255 / 255.0, 178 / 255.0, 69 / 255.0))));

	timer.Start();
	hitable** l = new hitable*[7];
	l[0] = left_wall;
	l[1] = right_wall;
	l[2] = bottom_floor;
	l[3] = top_ceiling;
	l[4] = back_wall;
	l[5] = light;
	l[6] = small_box;
	l[7] = large_box;
	timer.Stop();
	std::cout << "Took " << timer.GetElapsedSeconds() << " to load meshes." << std::endl;
	hitable* world = new bvh_node(l, 7, 0, 1);
	//camera cam(vec3(13, 2, 3), vec3(0, 0, 0), vec3(0, 1, 0), 20, float(nx) / ny, 0, 10, 0, 1);
	// camera for simple_light
	//camera cam(vec3(13, 2, 20), vec3(0, 0, 0), vec3(0, 1, 0), 40, float(nx) / ny, 0, 10, 0, 1);
	camera cam(vec3(278, 273, -800), vec3(278, 273, -799), vec3(0, 1, 0), 39.3077, float(nx) / ny, 0, 10, 0, 1);
	omp_set_nested(1);
	timer.Start();

#pragma omp parallel for schedule(dynamic, 1)
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
			bitmap.SetPixel(col, row, c);
		}
	}
	
	timer.Stop();
	std::cout << timer.GetElapsedSeconds() << std::endl;
	bitmap.SaveAsPPM6("image1.ppm");
	return 0;
}