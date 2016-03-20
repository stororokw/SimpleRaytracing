#include <iostream>
#include <fstream>
#include "Vec3.h"
#include "Ray.h"
#include "Hitable.h"
#include "Sphere.h"
#include "HitableList.h"

vec3 colour(const ray& r, hitable* world)
{
	hit_record rec;
	if (world->hit(r, 0.0f, INFINITY, rec))
	{
		vec3 normal = rec.normal;
		return 0.5f * (normal + vec3(1, 1, 1));
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
	std::ofstream output("image.ppm");

	vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
	vec3 horizontal(4.0f, 0.0f, 0.0f);
	vec3 vertical(0.0f, 2.0f, 0.0f);
	vec3 origin(0.0f, 0.0f, 0.0f);

	hitable* geometry[2];
	geometry[0] = new sphere(vec3(0, 0, -1), 0.5);
	geometry[1] = new sphere(vec3(0, -100.5, -1), 100);

	hitable* world = new hitable_list(geometry, 2);

	output << "P3\n" << nx << " " << ny << "\n255\n";
	for (int row = ny - 1; row >= 0; --row)
	{
		for (int col = 0; col < nx; ++col)
		{
			float u = float(col) / float(nx);
			float v = float(row) / float(ny);
			ray r(origin, lower_left_corner + u * horizontal + v * vertical);
			vec3 c = colour(r, world);
			int ir = int(255.99 * c[0]);
			int ig = int(255.99 * c[1]);
			int ib = int(255.99 * c[2]);
			output << ir << " " << ig << " " << ib << "\n";
		}
	}
	output.close();

	return 0;
}