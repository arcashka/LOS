#include "Solver.h"
#include "iostream"

#include "source/utils/MatrixUtils.h"
#include "source/LinearSystem.h"

Solver::Solver(const std::shared_ptr<LinearSystem> system) :
	ls(system)
{
}

bool Solver::Solve(std::vector<float> & x, const std::vector<float> & x0, float eps, int maxItt)
{
	std::vector<float> r = ls->b - ls->matrix * x0;
	std::vector<float> p = r;
	x = std::move(x0);

	float alpha;
	float beta;
	float bbScalar = FindScalar(ls->b, ls->b);
	float rrScalar1 = FindScalar(r, r);

	for (int k = 0; k < maxItt; k++)
	{
		float rrScalar = rrScalar1;
		auto ap = ls->matrix * p;
		alpha = rrScalar / FindScalar(ap, p);
		x = x + (alpha * p);
		r = r - (alpha * ap);
		rrScalar1 = FindScalar(r, r);
		beta =  rrScalar1 / rrScalar;
		p = r + (beta * p);

		float diff = rrScalar / bbScalar;
		std::cout << k + 1 << "\t" << "diff = " << diff << std::endl;

	if (diff < eps)
		break;
	}
	
	return true;
}
