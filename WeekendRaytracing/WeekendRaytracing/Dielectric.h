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
	
	vec3 outward_normal = rec.normal;
	float eta = 1 / ior;
	vec3 wo = unit_vector(r_in.direction());
	float costheta = dot(wo, rec.normal);
	attenuation = vec3(1, 1, 1);
	vec3 refracted;
	
	bool is_entering = dot(wo, rec.normal) < 0.0f;
	if (is_entering)
	{
		outward_normal = -rec.normal;
		eta = eta;
		costheta = -costheta;
	}

	float reflection_prob = schlick_fresnel(costheta, ior);
	
	vec3 reflected = reflect(wo, outward_normal);
	if (refract(wo, outward_normal, eta, refracted))
	{
		float f = 1 - reflection_prob;
		attenuation = vec3(f, f, f);
		scattered = ray(rec.p, refracted);
		return true;
	}
	attenuation = vec3(reflection_prob, reflection_prob, reflection_prob);
	scattered = ray(rec.p, reflected);
	return false;
}
