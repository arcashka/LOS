#include "Solver.h"
#include "iostream"

#include "source/utils/MatrixUtils.h"
#include "source/LinearSystem.h"

Solver::Solver(const std::shared_ptr<LinearSystem> system) :
	ls(system)
{
}

bool Solver::Solve(std::vector<double> & x, const std::vector<double> & x0, double eps, int maxItt)
{
	std::vector<double> r = ls->b - ls->matrix * x0;
	std::vector<double> p = r;
	x = std::move(x0);

	double alpha;
	double beta;
	double bbScalar = FindScalar(ls->b, ls->b);

	for (int k = 0; k < maxItt; k++)
	{
		double rrScalar = FindScalar(r, r);
		auto ap = ls->matrix * p;
		alpha = rrScalar / FindScalar(ap, p);
		x = x + (alpha * p);
		r = r - (alpha * ap);
		beta = FindScalar(r, r) / rrScalar;
		p = r + (beta * p);

		double diff = rrScalar / bbScalar;
		std::cout << k + 1 << "\t" << "diff = " << diff << std::endl;

	if (diff < eps)
		break;
	}
	
	return true;
}
