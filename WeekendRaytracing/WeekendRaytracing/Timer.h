#pragma once

#include <chrono>

class Timer
{
public:
	Timer()
	{}

	void Start()
	{
		StartTime = std::chrono::system_clock::now();
	}

	void Stop()
	{
		ElapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - StartTime).count();
	}

	float GetElapsedSeconds()
	{
		return (float)ElapsedTime / 1e6;
	}

	float GetElapsedMilliseconds()
	{
		return (float)ElapsedTime / 1e3;
	}

	float GetElapsedMicroseconds()
	{
		return (float)ElapsedTime;
	}
private:
	std::chrono::system_clock::time_point StartTime;
	long long ElapsedTime;

};