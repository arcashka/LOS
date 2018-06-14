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
	QCommandLineOption outOption(QStringList() << "o" << "out", "Write output (x) to file");
	QCommandLineOption sizeOption("size", "Matrix Dimension", "size");
	QCommandLineOption sparsenessOption("sparseness", "Matrix sparness. If set to n - only every n element will be set to none zero value", "sparseness");
	QCommandLineOption maxIttOption("maxItt", "Matrix Dimension", "Maximum itteration");

	parser.addOption(gpuOption);
	parser.addOption(sizeOption);
	parser.addOption(sparsenessOption);
	parser.addOption(maxIttOption);
	parser.addOption(outOption);
	parser.process(a);

	int defaultSize       = 100;
	int defaultSparseness = 10;
	int defaultMaxItt     = 40;

	bool useGPU      = parser.isSet(gpuOption);
	bool writeResult = parser.isSet(outOption);
	int  size        = parser.isSet(sizeOption)       ? parser.value(sizeOption).toInt()       : defaultSize;
	int  sparseness  = parser.isSet(sparsenessOption) ? parser.value(sparsenessOption).toInt() : defaultSparseness;
	int  maxItt      = parser.isSet(maxIttOption)     ? parser.value(maxIttOption).toInt()     : defaultMaxItt;

	IGenerator* generator = (IGenerator*)new Generator();

	auto CreateSolver = [](std::shared_ptr<Matrix> matrix, bool useGPU) -> ISolver* {
		if(useGPU)
			return new SolverGPU(matrix);
		else
			return new Solver(matrix);
	};	

	vector<double> xKnown;
	vector<double> x0(size);

	xKnown.reserve(size);
	for(int i = 0; i < size; i++)
		xKnown.push_back(i + 1);

	std::shared_ptr<Matrix> matrix;
	matrix = generator->Generate(size, sparseness, xKnown);
	std::cout << "Generated matrix with this parameters:\n" << "size: " << size << "\n" << "sparseness: " << sparseness << std::endl;

	ISolver* solver = CreateSolver(matrix, parser.isSet(gpuOption));
	std::vector<double> x;
	
	writeResult &= solver->Solve(x, x0, 1e-10, maxItt);
	
	if (writeResult)
	{
		for(auto elem : x)
			std::cout << elem << " ";
		std::cout << std::endl;
	}
}
