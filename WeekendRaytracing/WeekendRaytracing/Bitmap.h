#pragma once

#include "Vec3.h"
#include <vector>
#include <fstream>

class Bitmap
{
public:
	Bitmap(int width, int height, float gamma = 2.2);
	vec3* operator[](int x) const;
	vec3 GetPixel(int col, int row);
	void SetPixel(int col, int row, const vec3& colour);
	void SaveAsPPM(char* filepath);

protected:
	std::vector<vec3*> d;
	float gamma;
	int width;
	int height;
};

inline Bitmap::Bitmap(int width, int height, float gamma)
	: width(width), height(height), gamma(gamma)
{
	for (int row = 0; row < height; ++row)
	{
		d.push_back(new vec3[width]);
		for (int col = 0; col < width; ++col)
		{
			d[row][col] = vec3(0, 1, 0);
		}
	}
}

inline vec3* Bitmap::operator[](int x) const
{
	return d[x];
}

inline vec3 Bitmap::GetPixel(int col, int row)
{
	return d[row][col];
}

inline void Bitmap::SetPixel(int col, int row, const vec3& colour)
{
	d[row][col] = colour;
}

inline void Bitmap::SaveAsPPM(char* filepath)
{
	std::ofstream output(filepath);

	output << "P3\n" << width << " " << height << "\n255\n";
	for (int row = height - 1; row >= 0; --row)
	{
		for (int col = 0; col < width; ++col)
		{
			vec3 c = d[row][col];
			// gamma correct
			//c = vec3(sqrtf(c[0]), sqrtf(c[1]), sqrtf(c[2])).clamp();
			c = c ^ gamma;
			int ir = int(255.99 * c[0]);
			int ig = int(255.99 * c[1]);
			int ib = int(255.99 * c[2]);
			output << ir << " " << ig << " " << ib << "\n";
		}
	}
	output.close();

}
