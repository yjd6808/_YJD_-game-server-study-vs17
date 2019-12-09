#include "Clock.h"
#include <chrono>
#include <iostream>

using namespace std;
using namespace chrono;

std::chrono::time_point<std::chrono::system_clock> Clock::start;
std::chrono::time_point<std::chrono::system_clock> Clock::end;

void Clock::Start()
{
	start = chrono::system_clock::now();
}

void Clock::Stop()
{
	end = chrono::system_clock::now();
	auto dur = duration_cast<milliseconds>(end - start).count();
	cout << "Took " << dur << " miliseconds. " << endl;
}

void Clock::mStart()
{
	m_start = chrono::system_clock::now();
}

void Clock::mStop()
{
	m_end = chrono::system_clock::now();
	auto dur = duration_cast<milliseconds>(m_end - m_start).count();
	cout << "Took " << dur << " miliseconds. " << endl;
}
