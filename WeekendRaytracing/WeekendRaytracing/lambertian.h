#pragma once

#include "Material.h"
#include "Ray.h"
#include "RNG.h"
#include "Hitable.h"

class lambertian : public material
{
public:
	lambertian(vec3& a);
	bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override;

private:
	vec3 albedo;
};

inline lambertian::lambertian(vec3& a)
	: albedo(a)

{

}

inline bool lambertian::scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
{
	vec3 s = rec.p + rec.normal + RNG::random_in_unit_sphere();
	scattered = ray(rec.p, s - rec.p);
	attenuation = albedo;
	return true;
}
