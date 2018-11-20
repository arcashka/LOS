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
	GLuint x0;
	GLuint out;
};

struct SolverGPU::Impl {
	Impl(const std::shared_ptr<LinearSystem> system)
		: ls(system)
		, size(ls->b.size())
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
	}

	void SetSolverParameters(const std::vector<float> x0, int maxItt, float eps)
	{
		this->x0     = x0;
		this->maxItt = maxItt;
		this->eps    = eps;
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
		functions.glNamedBufferStorage(address, sizeof(float) * data.size(), data.data(), 0);
	}

	void TransferDataToGL()
	{
		program.bind();
		CreateReadBuffer(bufferAddresses.b,   ls->b);
		CreateReadBuffer(bufferAddresses.di,  ls->matrix.di);
		CreateReadBuffer(bufferAddresses.ggl, ls->matrix.ggl);
		CreateReadBuffer(bufferAddresses.ig,  ls->matrix.ig);
		CreateReadBuffer(bufferAddresses.jg,  ls->matrix.jg);
		CreateReadBuffer(bufferAddresses.x0,  x0);

		functions.glCreateBuffers(1, &bufferAddresses.out);
		functions.glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferAddresses.out);
		functions.glNamedBufferStorage(bufferAddresses.out, sizeof(float) * ls->b.size(), nullptr, GL_MAP_READ_BIT);

		GLint u_size   = program.uniformLocation("size");
		GLint u_eps    = program.uniformLocation("eps");
		GLint u_maxItt = program.uniformLocation("maxItt");

		program.setUniformValue(u_size,   size);
		program.setUniformValue(u_eps,    eps);
		program.setUniformValue(u_maxItt, maxItt);

		program.release();
	}

	void RunProgram()
	{
		program.bind();
		functions.glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, bufferAddresses.b);
		functions.glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, bufferAddresses.di);
		functions.glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, bufferAddresses.ggl);
		functions.glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, bufferAddresses.ig);
		functions.glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, bufferAddresses.jg);
		functions.glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, bufferAddresses.x0);
		functions.glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, bufferAddresses.out);

		functions.glDispatchCompute(1, 1, 1);
		program.release();
	}

	std::vector<float> ReadData()
	{
		float* mapped =
				reinterpret_cast<float*>(functions.glMapNamedBufferRange(bufferAddresses.out, 0, size, GL_MAP_READ_BIT));

		std::vector<float> out1;
		out1.insert(out1.end(), &mapped[0], &mapped[size]);

		functions.glUnmapNamedBuffer(bufferAddresses.out);
		return out1;
	}

	Addresses bufferAddresses;
	std::shared_ptr<LinearSystem> ls;

	QOpenGLShaderProgram      program;
	QOpenGLFunctions_4_5_Core functions;
	QOpenGLContext            context;
	QOffscreenSurface         surface;

	std::vector<float> x0;
	int                 size;
	int                 maxItt;
	float              eps;
};

SolverGPU::SolverGPU(const std::shared_ptr<LinearSystem> system)
	: impl(std::make_unique<Impl>(system))
{
}

bool SolverGPU::Solve(std::vector<float>& x, const std::vector<float>& x0, float eps, int maxItt)
{
	impl->SetSolverParameters(x0, maxItt, eps);
	impl->PrintGPUInfo();
	impl->TransferDataToGL();
	impl->RunProgram();
	x = impl->ReadData();

	return true;
}
