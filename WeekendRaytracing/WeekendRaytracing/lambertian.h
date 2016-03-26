#pragma once

#include "Material.h"
#include "Ray.h"
#include "RNG.h"
#include "Hitable.h"
#include "Texture.h"

class lambertian : public material
{
public:
	lambertian(texture* a);
	bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override;

private:
	texture* albedo;
};

inline lambertian::lambertian(texture* a)
	: albedo(a)

{

}

inline bool lambertian::scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
{
	vec3 s = rec.p + rec.normal + RNG::random_in_unit_sphere();
	scattered = ray(rec.p, s - rec.p, r_in.time());
	attenuation = albedo->value(0, 0, rec.p);
	return true;
}
