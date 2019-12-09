#pragma once

#include <chrono>

class Clock {
	static std::chrono::time_point<std::chrono::system_clock> start;
	static std::chrono::time_point<std::chrono::system_clock> end;

	std::chrono::time_point<std::chrono::system_clock> m_start;
	std::chrono::time_point<std::chrono::system_clock> m_end;
public:
	static void Start();
	static void Stop();

	void mStart();
	void mStop();
};
