#pragma once

#include <vector>
#include <memory>

class ISolver
{
public:
	virtual bool Solve(std::vector<double> & x, const std::vector<double> & x0, double eps, int maxItt) = 0;
};
