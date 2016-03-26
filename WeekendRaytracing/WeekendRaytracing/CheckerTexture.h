#pragma once
#include "Texture.h"

class checker_texture : public texture
{
public:
	checker_texture();
	checker_texture(texture* t0, texture* t1);
	vec3 value(float u, float v, const vec3& p) const override;
protected:
	texture* even;
	texture* odd;
};

inline checker_texture::checker_texture()
{
}

inline checker_texture::checker_texture(texture* t0, texture* t1)
	: even(t0), odd(t1)
{
}

inline vec3 checker_texture::value(float u, float v, const vec3& p) const
{
	float sines = sinf(10 * p[0]) * sinf(10 * p[1]) * sinf(10 * p[2]);
	if (sines < 0)
		return odd->value(u, v, p);
	return even->value(u, v, p);
}
