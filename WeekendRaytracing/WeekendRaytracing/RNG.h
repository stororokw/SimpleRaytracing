#pragma once

#include <random>

class RNG
{
public:
	static float random();
	static vec3 random_in_unit_sphere();
private:
	static std::random_device random_device;
	static std::mt19937 random_engine;
	static std::uniform_real_distribution<float> RNG::distribution;
};

inline float RNG::random()
{
	return distribution(random_device);
}

inline vec3 RNG::random_in_unit_sphere()
{
	vec3 p;
	do
	{
		// make sure point in unit cube is in the range of -1 to 1
		p = 2.0f * vec3(RNG::random(), RNG::random(), RNG::random()) - vec3(1, 1, 1);
	} while (dot(p, p) >= 1.0f);
	return p;
}

std::random_device RNG::random_device;
std::mt19937 RNG::random_engine(random_device());
std::uniform_real_distribution<float> RNG::distribution = std::uniform_real_distribution<float>(0, 1);
