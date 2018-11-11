#pragma once

#include <memory>
#include <chrono>
#include <string>

class Timer
{
public:
	Timer() {};
	~Timer() {};
	void Start();
	std::string Result();

private:
	bool m_isStarted = false;
	std::chrono::time_point<std::chrono::steady_clock> m_start;
};

