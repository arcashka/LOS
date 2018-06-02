#include <QApplication>

#include "solver.h"
#include "solvergpu.h"
#include "matrix.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	Matrix matrix = {
		std::vector<double>{1, 3, 2, 7},
		std::vector<double>{5, 4},
		std::vector<int>   {0, 0, 0, 1, 2},
		std::vector<int>   {0, 0}
	};

	std::vector<double> b =  {32, 6, 11, 32};
	std::vector<double> x0 = {0, 0, 0, 0};

//	Solver solver(std::make_shared<Matrix>(matrix), b);
	SolverGPU solverGPU(std::make_shared<Matrix>(matrix), b);
//	solver.Solve(x0, 1e-10, 40);
	solverGPU.Solve(x0, 1e-10, 40);

//	return a.exec();
}
