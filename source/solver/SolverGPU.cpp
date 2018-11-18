#include "SolverGPU.h"

#include <iostream>

#include <QOffscreenSurface>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>

#include "source/LinearSystem.h"

struct Addresses {
	GLuint b;
	GLuint di;
	GLuint ggl;
	GLuint ig;
	GLuint jg;
	GLuint out;
};

struct SolverGPU::Impl {
	Impl(const std::shared_ptr<LinearSystem> system)
		: ls(system)
	{
		QSurfaceFormat format;
		format.setMajorVersion(4);
		format.setMinorVersion(5);
		format.setProfile(QSurfaceFormat::CoreProfile);

		context.setFormat(format);
		context.create();

		surface.create();
		context.makeCurrent(&surface);

		functions.initializeOpenGLFunctions();

		program.addShaderFromSourceFile(QOpenGLShader::Compute, ":/shaders/compute.glsl");
		program.link();
		program.bind();
	}

	void PrintGPUInfo()
	{
		int maxWorkInvocations, maxSharedMemorySize;
		int maxWorkSize[3];

		functions.glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &maxWorkSize[0]);
		functions.glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &maxWorkSize[1]);
		functions.glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &maxWorkSize[2]);

		functions.glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &maxWorkInvocations);
		functions.glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE,     &maxSharedMemorySize);

		std::cout << "GL_MAX_COMPUTE_WORK_GROUP_SIZE\t" << "x: " << maxWorkSize[0] << "\ty: " << maxWorkSize[1] << "\tz: " << maxWorkSize[2] << std::endl;
		std::cout << "GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS\t" << maxWorkInvocations << std::endl;
		std::cout << "GL_MAX_COMPUTE_SHARED_MEMORY_SIZE\t" << maxSharedMemorySize << std::endl;
	}

	template<typename T>
	void CreateReadBuffer(GLuint& address, std::vector<T>& data) {
		functions.glCreateBuffers(1, &address);
		functions.glBindBuffer(GL_SHADER_STORAGE_BUFFER, address);
		functions.glNamedBufferStorage(address, data.size(), data.data(), 0);
	}

	void CreateBuffers()
	{
//		CreateReadBuffer(bufferAddresses.b,   ls->b);
//		CreateReadBuffer(bufferAddresses.di,  ls->matrix.di);
//		CreateReadBuffer(bufferAddresses.ggl, ls->matrix.ggl);
//		CreateReadBuffer(bufferAddresses.ig,  ls->matrix.ig);
//		CreateReadBuffer(bufferAddresses.jg,  ls->matrix.jg);
		auto b = std::vector<double>(100, 20);
		CreateReadBuffer(bufferAddresses.b,   b);
		CreateReadBuffer(bufferAddresses.di,  b);
		CreateReadBuffer(bufferAddresses.ggl, ls->matrix.ggl);
		CreateReadBuffer(bufferAddresses.ig,  ls->matrix.ig);
		CreateReadBuffer(bufferAddresses.jg,  ls->matrix.jg);

		functions.glCreateBuffers(1, &bufferAddresses.out);
		functions.glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferAddresses.out);
		functions.glNamedBufferStorage(bufferAddresses.out, ls->matrix.di.size(), nullptr, GL_MAP_READ_BIT);
	}

	void RunProgram()
	{
		functions.glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, bufferAddresses.b);
		functions.glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, bufferAddresses.di);
		functions.glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, bufferAddresses.ggl);
		functions.glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, bufferAddresses.ig);
		functions.glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, bufferAddresses.jg);
		functions.glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, bufferAddresses.out);

		functions.glDispatchCompute(1, 1, 1);
	}

	std::vector<double> ReadData()
	{
		double* mapped =
				reinterpret_cast<double*>(functions.glMapNamedBufferRange(bufferAddresses.out, 0, 100, GL_MAP_READ_BIT));

		std::vector<double> out1;
		out1.insert(out1.end(), &mapped[0], &mapped[ls->b.size()]);

		functions.glUnmapNamedBuffer(bufferAddresses.out);
		return out1;
	}

	Addresses bufferAddresses;
	std::shared_ptr<LinearSystem> ls;

	QOpenGLShaderProgram      program;
	QOpenGLFunctions_4_5_Core functions;
	QOpenGLContext            context;
	QOffscreenSurface         surface;
};

SolverGPU::SolverGPU(const std::shared_ptr<LinearSystem> system)
	: impl(std::make_unique<Impl>(system))
{
}

bool SolverGPU::Solve(std::vector<double>& x, const std::vector<double>& x0, double eps, int maxItt)
{
	impl->PrintGPUInfo();
	impl->CreateBuffers();
	impl->RunProgram();
	auto a = impl->ReadData();

	return true;
}
