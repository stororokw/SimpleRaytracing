#pragma once

#include <random>

class RNG
{
public:
	static float random();
private:
	static std::random_device random_device;
	static std::mt19937 random_engine;
	static std::uniform_real_distribution<float> RNG::distribution;
};

inline float RNG::random()
{
	return distribution(random_device);
}

std::random_device RNG::random_device;
std::mt19937 RNG::random_engine(random_device());
std::uniform_real_distribution<float> RNG::distribution = std::uniform_real_distribution<float>(0, 1);
