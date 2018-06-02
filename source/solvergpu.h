#pragma once

#include <QObject>

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_5_Core>

#include <memory>
#include <vector>

struct Matrix;

using namespace std;

class SolverGPU : public QObject
{
	Q_OBJECT
public:
	SolverGPU(shared_ptr<Matrix> a, vector<double>& b);
//	virtual ~SolverGPU(){};
	vector<double> Solve(vector<double>& x0, double eps, int maxItt);

private:
	shared_ptr<Matrix> a;
	vector<double>& b;

private:
	QOpenGLShaderProgram *program;
};
