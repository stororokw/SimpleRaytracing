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
	
	vec3 outward_normal;
	float eta;						   
	vec3 wo = unit_vector(r_in.direction());
	float costheta = dot(wo, rec.normal);
	attenuation = vec3(1, 1, 1);
	vec3 refracted;
	
	bool is_entering = dot(wo, rec.normal) < 0.0f;
	if (!is_entering)
	{
		outward_normal = -rec.normal;
		eta = ior;
		costheta = -costheta;
	}
	else
	{
		eta = 1 / ior;
		outward_normal = rec.normal;
	}
	vec3 reflected = reflect(-wo, outward_normal);

	float reflection_prob = schlick_fresnel(costheta, eta);
	if (refract(wo, outward_normal, eta, refracted))
	{
		scattered = ray(rec.p, refracted, r_in.time());
	}

	if (RNG::random() < reflection_prob)
	{
		scattered = ray(rec.p, reflected, r_in.time());
		return (dot(scattered.direction(), rec.normal) > 0.0f);
	}
	
	return true;
}
