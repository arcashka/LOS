#pragma once

#include <memory>
#include <vector>

#include "ISolver.h"

struct LinearSystem;

class Solver : public ISolver
{
public:
	Solver(const std::shared_ptr<LinearSystem> system);

	// x0 - начальное приближение, maxItt - максимальное количество иттераций
	bool Solve(std::vector<double> & x, const std::vector<double> & x0, double eps, int maxItt) override;

private:
	std::shared_ptr<LinearSystem> ls;
};
