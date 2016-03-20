#pragma once

#include "Material.h"
#include "Ray.h"
#include "Hitable.h"
#include "Vec3.h"

class dielectric : public material
{
public:
	dielectric(float ior);
	bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override;

private:
	float ior;
};

inline dielectric::dielectric(float ior)
	: ior(ior)
{

}

inline bool dielectric::scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
{
	vec3 reflected = reflect(r_in.direction(), rec.normal);
	
	vec3 outward_normal = rec.normal;
	float eta = ior;

	attenuation = vec3(1, 1, 1);
	vec3 refracted;
	
	bool is_entering = dot(r_in.direction(), rec.normal) > 0.0f;
	if (!is_entering)
	{
		outward_normal = -rec.normal;
		eta = 1 / ior;
	}

	if (refract(r_in.direction(), outward_normal, eta, refracted))
	{
		scattered = ray(rec.p, refracted);
		return true;
	}
	scattered = ray(rec.p, reflected);
	return true;
}
