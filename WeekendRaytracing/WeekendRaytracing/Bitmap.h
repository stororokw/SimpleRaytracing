#pragma once

#include "Vec3.h"
#include <vector>
#include <fstream>
#include <string>

class Bitmap
{
public:
	Bitmap(int width, int height);
	int GetWidth() const;
	int GetHeight() const;
	vec3* operator[](int x) const;
	vec3 GetPixel(int col, int row);
	void SetPixel(int col, int row, const vec3& colour);
	void SaveAsPPM3(char* filepath, float gamma = 1 / 2.2);
	void SaveAsPPM6(char* filepath, float gamma = 1 / 2.2);
	static Bitmap* LoadPPM6(char* filepath, float gamma = 2.2);
protected:
	std::vector<vec3*> d;
	float gamma;
	int width;
	int height;
};

inline Bitmap::Bitmap(int width, int height)
	: width(width), height(height)
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

inline int Bitmap::GetWidth() const
{
	return width;
}

inline int Bitmap::GetHeight() const
{
	return height;
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

inline void Bitmap::SaveAsPPM3(char* filepath, float gamma)
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

// Saves binary ppm file (magic number 6)
inline void Bitmap::SaveAsPPM6(char* filepath, float gamma)
{
	std::ofstream ofs(filepath, std::ios::out | std::ios::binary);
	ofs << "P6" << std::endl;
	ofs << width << std::endl;
	ofs << height << std::endl;
	ofs << 255 << std::endl;

	vec3 c;
	unsigned char r, g, b;
	for (int row = height - 1; row >= 0; --row)
	{
		for (int col = 0; col < width; ++col)
		{
			c = d[row][col];
			c = c ^ gamma;
			r = c[0] * 255;
			g = c[1] * 255;
			b = c[2] * 255;
			ofs << r << g << b;
		}
	}

	ofs.close();
}

// Loads binary ppm file (magic number 6)
inline Bitmap* Bitmap::LoadPPM6(char* filepath, float gamma)
{
	std::ifstream ifs(filepath, std::ios::binary | std::ios::in);

	if (!ifs)
	{
		std::cerr << "LoadPPM: Could not open " << filepath << std::endl;
	}
	std::string format;
	int Width, Height;
	int MaxGray;
	ifs >> format;
	ifs >> Width >> Height;
	ifs >> MaxGray;
	Bitmap* result = new Bitmap(Width, Height);

	std::string Line;
	unsigned char buffer[3];

	// eat whitespace
	while (ifs.peek() == '\n' || ifs.peek() == ' ')
	{
		ifs.read((char*)(&buffer[0]), 1);
	}
	for (int row = Height - 1; row >= 0; --row)
	{
		for (int col = 0; col < Width; ++col)
		{
			ifs.read((char*)(&buffer[0]), sizeof(buffer));
			int r = (int)(unsigned char)buffer[0];
			int g = (int)(unsigned char)buffer[1];
			int b = (int)(unsigned char)buffer[2];
			//cout << r << " " << g << " " << b << endl;
			vec3 pixel = vec3(r / (float)MaxGray, g / (float)MaxGray, b / (float)MaxGray) ^ gamma;
			result->SetPixel(col, row, pixel);

		}
	}

	ifs.close();
	return result;
}
