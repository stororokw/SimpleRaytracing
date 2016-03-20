#include <iostream>
#include <fstream>
#include "Vec3.h"

int main()
{
	int nx = 200;
	int ny = 100;
	std::ofstream output("image.ppm");
	output << "P3\n" << nx << " " << ny << "\n255\n";
	for (int row = ny - 1; row >= 0; --row)
	{
		for (int col = 0; col < nx; ++col)
		{
			vec3 c(float(col) / float(nx), float(row) / float(ny), 0.2);
			int ir = int(255.99 * c[0]);
			int ig = int(255.99 * c[1]);
			int ib = int(255.99 * c[2]);
			output << ir << " " << ig << " " << ib << "\n";
		}
	}
	output.close();

	return 0;
}