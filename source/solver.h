#pragma once

#include <memory>
#include <vector>

struct Matrix;

using namespace std;

class Solver
{
public:
	Solver(shared_ptr<Matrix> a, vector<double>& b);

	// x0 - начальное приближение, maxItt - максимальное количество иттераций
	vector<double>  Solve(vector<double>& x0, double eps, int maxItt);

private:
	// скалярное произведение векторов vec1 и vec2
	double          FindScalar(const vector<double> &vec1, const vector<double> &vec2);

private:
	shared_ptr<Matrix> a;
	vector<double>& b;
};
