#pragma once

#include <QObject>

#include <QOpenGLShaderProgram>
//#include <QOpenGLFunctions>

#include <memory>
#include <vector>

struct Matrix;

using namespace std;

class SolverGPU : QObject
//				 ,protected QOpenGLFunctions
{
	Q_OBJECT
public:
	SolverGPU(shared_ptr<Matrix> a, vector<double>& b);
	vector<double> Solve(vector<double>& x0, double eps, int maxItt);

private:
	shared_ptr<Matrix> a;
	vector<double>& b;

private:
	QOpenGLShaderProgram *program;
};
