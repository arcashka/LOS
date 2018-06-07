#pragma once

#include <vector>
#include <memory>

class ISolver
{
public:
	virtual std::vector<double> Solve(std::vector<double>& x0, double eps, int maxItt) = 0;
};
