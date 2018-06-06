#pragma once

#include <QObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_5_Core>

#include <memory>
#include <vector>

#include "ISolver.h"

struct Matrix;

using namespace std;

class SolverGPU : public QObject, public ISolver
{
	Q_OBJECT
public:
	SolverGPU(shared_ptr<Matrix> a, vector<double>& b);

	vector<double> Solve(vector<double>& x0, double eps, int maxItt) override;

private:
	shared_ptr<Matrix> a;
	vector<double>& b;

private:
	QOpenGLShaderProgram *program;
};
