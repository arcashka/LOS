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
	// скалярное произведение векторов vec1 и vec2
	double          FindScalar(const vector<double> &vec1, const vector<double> &vec2);

private:
	shared_ptr<Matrix> a;
};
