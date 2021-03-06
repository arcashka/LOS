#include <QApplication>
#include <QCommandLineParser>

#include "solver/Solver.h"
#include "solver/SolverGPU.h"
#include "Matrix.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QCommandLineParser parser;
	parser.addHelpOption();

	QCommandLineOption gpuOption("gpu", "If set use gpu for comptutations");
	parser.addOption(gpuOption);
	parser.process(a);

	Matrix matrix = {
		std::vector<double>{1, 3, 2, 7},
		std::vector<double>{5, 4},
		std::vector<int>   {0, 0, 0, 1, 2},
		std::vector<int>   {0, 0}
	};

	std::vector<double> b =  {32, 6, 11, 32};
	std::vector<double> x0 = {0, 0, 0, 0};

	auto CreateSolver = [](std::shared_ptr<Matrix> matrix, std::vector<double> & b, bool useGPU) -> ISolver* {
		if(useGPU)
			return new SolverGPU(matrix, b);
		else
			return new Solver(matrix, b);
	};

	ISolver* solver = CreateSolver(std::make_shared<Matrix>(matrix), b, parser.isSet("gpuOption"));
	solver->Solve(x0, 1e-10, 40);

//	return a.exec();
}
