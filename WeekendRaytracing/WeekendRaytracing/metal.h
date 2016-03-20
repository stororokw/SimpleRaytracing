#pragma once

#include "Material.h"
#include "Ray.h"
#include "RNG.h"
#include "Hitable.h"

class metal : public material
{
public:
	metal(vec3& a, float fuzziness);
	bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override;

private:
	vec3 albedo;
	float fuzziness;
};

inline metal::metal(vec3& a, float f)
	: albedo(a),
	  fuzziness(f)
{
	if (fuzziness > 1.0f)
	{
		fuzziness = 1.0f;
	}
}

inline bool metal::scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
{
	vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
	scattered = ray(rec.p, reflected + fuzziness * RNG::random_in_unit_sphere());
	attenuation = albedo;
	return (dot(scattered.direction(), rec.normal) > 0.0f);
}
