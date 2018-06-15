#include "SolverGPU.h"
#include "iostream"

#include <QOffscreenSurface>

#include "source/Matrix.h"

SolverGPU::SolverGPU(const std::shared_ptr<LinearSystem> system)
	: ls(system)
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

	std::cout << "GL_MAX_COMPUTE_WORK_GROUP_SIZE\t" << "x: " << maxWorkSize[0] << "\ty: " << maxWorkSize[1] << "\tz: " << maxWorkSize[2] << std::endl;
	std::cout << "GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS\t" << maxWorkInvocations << std::endl;
	std::cout << "GL_MAX_COMPUTE_SHARED_MEMORY_SIZE\t" << maxSharedMemorySize << std::endl;
}

bool SolverGPU::Solve(std::vector<double>& x, const std::vector<double>& x0, double eps, int maxItt)
{
	auto pew = eps + maxItt;
	pew += x0.size();
	std::vector<double> a;
	x = x0;
	return true;
}
