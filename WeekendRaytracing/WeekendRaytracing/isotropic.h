#pragma once
#include "Material.h"
#include "Texture.h"
#include "RNG.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Ray.h"
#include "Hitable.h"

class isotropic : public material
{
public:
	isotropic(texture* emission);
	bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override;
	inline vec3 emitted(float u, float v, const vec3& p) const override;
protected:
	texture* emission;
};

inline isotropic::isotropic(texture* emission)
	: emission(emission)
{

}

inline bool isotropic::scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
{
	attenuation = vec3(1, 1, 1) * emission->value(rec.u, rec.v, rec.p);
	scattered = ray(rec.p, RNG::random_in_unit_sphere());
	return true;
}

inline vec3 isotropic::emitted(float u, float v, const vec3& p) const
{
	return vec3(0, 0, 0);
}
