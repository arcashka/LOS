#include <QApplication>
#include <QCommandLineParser>

#include <iostream>

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
	QCommandLineOption sizeOption("size", "Matrix Dimension", "size");
	QCommandLineOption sparsenessOption("sparseness", "Matrix sparness. If set to n - only every n element will be set to none zero value", "sparseness");

	parser.addOption(gpuOption);
	parser.addOption(sizeOption);
	parser.addOption(sparsenessOption);
	parser.process(a);

	int defaultSize = 100;
	int defaultSparseness = 10;

	bool useGPU     = parser.isSet(gpuOption);
	int  size       = parser.isSet(sizeOption) ? parser.value(sizeOption).toInt() : defaultSize;
	int  sparseness = parser.isSet(sparsenessOption) ? parser.value(sparsenessOption).toInt() : defaultSparseness;

	IGenerator* generator = (IGenerator*)new Generator();

	auto CreateSolver = [](std::shared_ptr<Matrix> matrix, bool useGPU) -> ISolver* {
		if(useGPU)
			return new SolverGPU(matrix);
		else
			return new Solver(matrix);
	};	

	vector<double> x;
	vector<double> x0(size);

	x.reserve(size);
	for(int i = 0; i < size; i++)
		x.push_back(i + 1);

	std::shared_ptr<Matrix> matrix;
	try {
		matrix = generator->Generate(size, sparseness, x);
	}
	catch(std::runtime_error& e)
	{
		std::cout << "Failed to generate matrix with this parameters:\n" << "size: " << size << "\n" << "sparseness: " << sparseness << std::endl;
		std::cout << "Reason: " << e.what() << std::endl;
	}
	std::cout << "Generated matrix with this parameters:\n" << "size: " << size << "\n" << "sparseness: " << sparseness << std::endl;

	ISolver* solver = CreateSolver(matrix, parser.isSet(gpuOption));
	solver->Solve(x0, 1e-10, 40);
}
