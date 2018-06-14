#include "Solver.h"
#include "iostream"

#include "source/matrixGenerator/Matrix.h"
#include "MatrixUtils.h"

Solver::Solver(const shared_ptr<Matrix> a) :
	a(a)
{
}

bool Solver::Solve(vector<double> & x, const vector<double> & x0, double eps, int maxItt)
{
	vector<double> r = a->b - a * x0;
	vector<double> p = r;
	x = x0;

	double alpha;
	double beta;
	double bbScalar = FindScalar(a->b, a->b);

	for (int k = 0; k < maxItt; k++)
	{
		double rrScalar = FindScalar(r, r);
		alpha = rrScalar / FindScalar(a * p, p);
		x = x + (alpha * p);
		r = r - (alpha * (a * p));
		beta = FindScalar(r, r) / rrScalar;
		p = r + (beta * p);

		double diff = rrScalar / bbScalar;
		cout << k + 1 << "\t" << "diff = " << diff << std::endl;

	if (diff < eps)
			break;
	}
	
	return true;
}
