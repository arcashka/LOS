#pragma once

#include <vector>
#include <memory>

class ISolver
{
public:
	virtual bool Solve(std::vector<float>& x, const std::vector<float>& x0, float eps, int maxItt, int & realItt) = 0;
};
