#pragma once

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_5_Core>

#include <memory>
#include <vector>

#include "ISolver.h"

struct LinearSystem;

class SolverGPU : public QObject, public ISolver
{
	Q_OBJECT
public:
	SolverGPU(const std::shared_ptr<LinearSystem> ls);

	bool Solve(std::vector<double> & x, const std::vector<double> & x0, double eps, int maxItt) override;

private:
	std::shared_ptr<LinearSystem> ls;
	std::unique_ptr<QOpenGLShaderProgram> program;
};
