#pragma once

#include "Vec3.h"

struct hit_record;
class ray;

class material
{
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
	static vec3 reflect(const vec3& v, const vec3& n);
	static bool refract(const vec3& v, const vec3& n, float ior, vec3& refracted);
	static float schlick_fresnel(float costheta, float ior)
	{
		// air
		float eta_i = 1.0f;
		float eta_t = ior / eta_i;
		// probability of reflection at normal incidence
		float r0 = powf((eta_i - eta_t) / (eta_i + eta_t), 2.0f);
		return r0 + (1 - r0) * powf(1 - fabsf(costheta), 5.0f);
	}
};

inline vec3 material::reflect(const vec3& v, const vec3& n)
{
	return v - 2 * dot(v, n) * n;
}

// refer to realistic ray tracing pg 176-177 for derivation
inline bool material::refract(const vec3& v, const vec3& normal, float eta, vec3& refracted)
{
	vec3 uv = unit_vector(v);
	float dt = dot(uv, normal);
	float discriminant = 1.0 - eta * eta * (1 - dt * dt);
	// total internal reflection
	if (discriminant <= 0)
	{
		return false;
	}

	refracted = eta * (v - normal * dt) - normal * sqrtf(discriminant);
	return true;
}


