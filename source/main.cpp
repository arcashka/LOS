#include <QGuiApplication>
#include <QCommandLineParser>

#include <iostream>

#include "source/solver/CreateSolver.h"
#include "source/solver/ISolver.h"

#include "source/matrixGenerator/CreateGenerator.h"

#include "source/timer/Timer.h"
#include "LinearSystem.h"

struct UserSettings
{
	bool useGPU;
	bool writeResult;
	int  size;
	int  sparseness;
	int  maxItt;
};

namespace {
	constexpr int DEFAULT_SIZE       = 100;
	constexpr int DEFAULT_SPARSENESS = 10;
	constexpr int DEFAULT_MAX_ITT    = 40;
}

void ProcessCmdArgs(UserSettings& settings, const QStringList& args)
{
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

	parser.process(args);

	settings.useGPU      = parser.isSet(gpuOption);
	settings.writeResult = parser.isSet(outOption);
	settings.size        = parser.isSet(sizeOption)       ? parser.value(sizeOption).toInt()       : DEFAULT_SIZE;
	settings.sparseness  = parser.isSet(sparsenessOption) ? parser.value(sparsenessOption).toInt() : DEFAULT_SPARSENESS;
	settings.maxItt      = parser.isSet(maxIttOption)     ? parser.value(maxIttOption).toInt()     : DEFAULT_MAX_ITT;
}

void WriteResult(const std::vector<double> & x)
{
	for(auto elem : x)
		std::cout << elem << " ";
	std::cout << std::endl;
}

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);

	UserSettings settings;
	ProcessCmdArgs(settings, app.arguments());

	std::vector<double> xKnown;
	std::vector<double> x0(settings.size);
	std::vector<double> x;
	xKnown.reserve(settings.size);
	for(int i = 0; i < settings.size; i++)
		xKnown.push_back(i + 1);

	std::shared_ptr<LinearSystem> system = CreateGenerator()->Generate(settings.size, settings.sparseness, xKnown);
	ISolver* solver = CreateSolver(system, settings.useGPU);

	Timer timer;
	timer.Start();
	settings.writeResult &= solver->Solve(x, x0, 1e-10, settings.maxItt);
	std::string time = timer.Result();

	if (settings.writeResult)
		WriteResult(x);
	std::cout << "Time: " << time << std::endl;
}
