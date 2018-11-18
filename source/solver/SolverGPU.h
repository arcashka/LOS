#pragma once

#include <memory>
#include <vector>

#include "ISolver.h"

struct LinearSystem;

class SolverGPU : public ISolver
{
public:
	SolverGPU(const std::shared_ptr<LinearSystem> ls);

	bool Solve(std::vector<double> & x, const std::vector<double> & x0, double eps, int maxItt) override;

private:
	struct Impl;
	std::unique_ptr<Impl> impl;
};
