#include <QApplication>
#include <QCommandLineParser>

#include "solver/Solver.h"
#include "solver/SolverGPU.h"
#include "matrixGenerator/Generator.h"
#include "matrixGenerator/Matrix.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QCommandLineParser parser;
	parser.addHelpOption();

	QCommandLineOption gpuOption("gpu", "If set use gpu for comptutations");
	parser.addOption(gpuOption);
	parser.process(a);

	IGenerator* generator = (IGenerator*)new Generator();
	std::vector<double> x0 = {0, 0, 0, 0};

	auto CreateSolver = [](std::shared_ptr<Matrix> matrix, bool useGPU) -> ISolver* {
		if(useGPU)
			return new SolverGPU(matrix);
		else
			return new Solver(matrix);
	};	

	ISolver* solver = CreateSolver(generator->Generate(0), parser.isSet(gpuOption));
	solver->Solve(x0, 1e-10, 40);
}
