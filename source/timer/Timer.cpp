#include "Timer.h"

#include <string>

void Timer::Start()
{
	if(m_isStarted)
		return;
	m_start = std::chrono::high_resolution_clock::now();
	m_isStarted = true;
}

std::string Timer::Result()
{
	const auto end = std::chrono::high_resolution_clock::now();
	const double ticks = std::chrono::duration<double, std::milli>(end - m_start).count();
	const std::string time = std::to_string(ticks) + "ms";
	return time;
}
