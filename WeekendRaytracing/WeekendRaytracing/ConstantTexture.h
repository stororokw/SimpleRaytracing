#pragma once

#include "Texture.h"

class constant_texture : public texture
{
public:
	constant_texture();
	constant_texture(vec3 c);
	vec3 value(float u, float v, const vec3& p) const override;

protected:
	vec3 colour;
};

inline constant_texture::constant_texture()
{

}

inline constant_texture::constant_texture(vec3 c)
	: colour(c)
{

}

inline vec3 constant_texture::value(float u, float v, const vec3& p) const
{
	return colour;
}
