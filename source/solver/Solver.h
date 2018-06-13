#pragma once

#include <memory>
#include <vector>

#include "ISolver.h"

struct Matrix;

using namespace std;

class Solver : public ISolver
{
public:
	Solver(shared_ptr<Matrix> a);

	// x0 - начальное приближение, maxItt - максимальное количество иттераций
	vector<double>  Solve(vector<double>& x0, double eps, int maxItt) override;

private:
	shared_ptr<Matrix> a;
};
