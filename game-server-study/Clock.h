#pragma once

#include <chrono>

class Clock {
	static std::chrono::time_point<std::chrono::system_clock> start;
	static std::chrono::time_point<std::chrono::system_clock> end;
public:
	static void Start();
	static void Stop();
};
