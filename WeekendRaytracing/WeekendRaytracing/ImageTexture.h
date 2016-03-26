#pragma once

#include "Texture.h"
#include "Bitmap.h"

class image_texture : public texture
{
public:
	image_texture();
	image_texture(Bitmap* bitmap);
	vec3 value(float u, float v, const vec3& p) const override;

protected:
	Bitmap* bitmap;
};

inline image_texture::image_texture()
{
}

inline image_texture::image_texture(Bitmap* bitmap)
	: bitmap(bitmap)
{
}

inline vec3 image_texture::value(float u, float v, const vec3& p) const
{
	int col = bitmap->GetWidth() * u;
	int row = bitmap->GetHeight() * v;
	col = clampf(col, 0, bitmap->GetWidth() - 1);
	row = clampf(row, 0, bitmap->GetHeight() - 1);
	return bitmap->GetPixel(col, row);
}
