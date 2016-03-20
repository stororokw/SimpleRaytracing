#include <iostream>
#include <fstream>

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
			float u = float(col) / float(nx);
			float v = float(row) / float(ny);

			float r = float(col) / float(nx);
			float g = float(row) / float(ny);
			float b = 0.2;
			int ir = int(255.99 * r);
			int ig = int(255.99 * g);
			int ib = int(255.99 * b);
			output << ir << " " << ig << " " << ib << "\n";
		}
	}
	output.close();

	return 0;
}