#include "SolverGPU.h"
#include "fstream"
#include "iostream"

#include <QOffscreenSurface>
#include <QTimer>
#include <QEventLoop>

#include "source/matrixGenerator/Matrix.h"

SolverGPU::SolverGPU(const shared_ptr<Matrix> a)
	: a(a)
{
	QSurfaceFormat format;
	format.setMajorVersion(4);
	format.setMinorVersion(5);
	format.setProfile(QSurfaceFormat::CoreProfile);

	QOpenGLContext context;
	context.setFormat(format);
	context.create();

	QOffscreenSurface surface;
	surface.create();
	context.makeCurrent(&surface);

	QOpenGLFunctions_4_5_Core functions;
	functions.initializeOpenGLFunctions();
	program = new QOpenGLShaderProgram();
	program->addShaderFromSourceFile(QOpenGLShader::Compute, ":/shaders/compute.glsl");
	program->link();
	program->bind();
	int maxWorkInvocations, maxSharedMemorySize;
	int maxWorkSize[3];

	functions.glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &maxWorkSize[0]);
	functions.glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &maxWorkSize[1]);
	functions.glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &maxWorkSize[2]);

	functions.glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &maxWorkInvocations);
	functions.glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &maxSharedMemorySize);

	std::cout << "GL_MAX_COMPUTE_WORK_GROUP_SIZE\t" << "x: " << maxWorkSize[0] << "\ty: " << maxWorkSize[1] << "\tz: " << maxWorkSize[2] << endl;
	std::cout << "GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS\t" << maxWorkInvocations << endl;
	std::cout << "GL_MAX_COMPUTE_SHARED_MEMORY_SIZE\t" << maxSharedMemorySize << endl;
}

bool SolverGPU::Solve(vector<double> & x, const vector<double> & x0, double eps, int maxItt)
{
	auto pew = eps + maxItt;
	pew += x0.size();
	std::vector<double> a;
	x = x0;
	return true;
}
