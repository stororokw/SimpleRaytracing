#pragma once

#include "Material.h"
#include "Texture.h"

class emitter_material : public material
{
public:
	emitter_material(texture* a);
	bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override;
	virtual vec3 emitted(float u, float v, const vec3& p) const override;
protected:
	texture* emitter;
};

inline emitter_material::emitter_material(texture* a)
	: emitter(a)
{
}

inline bool emitter_material::scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
{
	return false;
}

inline vec3 emitter_material::emitted(float u, float v, const vec3& p) const
{
	return emitter->value(u, v, p);
}
