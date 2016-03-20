#pragma once

#include "Material.h"
#include "Ray.h"
#include "RNG.h"
#include "Hitable.h"

class metal : public material
{
public:
	metal(vec3& a);
	bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override;

private:
	vec3 albedo;
};

inline metal::metal(vec3& a)
	: albedo(a)

{

}

inline bool metal::scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
{
	vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
	scattered = ray(rec.p, reflected);
	attenuation = albedo;
	return (dot(scattered.direction(), rec.normal) > 0.0f);
}
