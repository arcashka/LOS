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
	bool Solve(std::vector<float> & x, const std::vector<float> & x0, float eps, int maxItt, int & realItt) override;

private:
	std::shared_ptr<LinearSystem> ls;
};
