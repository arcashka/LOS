#include "solvergpu.h"
#include "iostream"

SolverGPU::SolverGPU(shared_ptr<Matrix> a, vector<double>& b)
	: a(a), b(b)
{
//	initializeOpenGLFunctions();
	program = new QOpenGLShaderProgram();
	QOpenGLShader shader(QOpenGLShader::Compute);
	bool compile = shader.compileSourceFile(":/shaders/compute.glsl");
	bool add = program->addShader(&shader);
	auto log = program->log();

	std::cout << "\n" << "Compile" << compile << "\t" << "add " << add << "\t";
	std::cout << "log " << log.toStdString() << "\n";

//	program->addShaderFromSourceFile(QOpenGLShader::Compute, ":/shaders/compute.glsl");

	program->link();
	program->bind();
}

vector<double> SolverGPU::Solve(vector<double>& x0, double eps, int maxItt)
{
	auto pew = eps + maxItt;
	pew += x0.size();
	std::vector<double> a;
	return a;
}
