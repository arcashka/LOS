#version 450 core
#extension GL_NV_shader_atomic_float : enable

// TODO: CONFIGURE THIS CONST
const int cs = 3;

layout (local_size_x = cs) in;

layout (binding = 0) buffer bBuffer
{
	double v[cs];
} b;
layout (binding = 1) buffer diBuffer
{
	double v[cs];
} di;
layout (binding = 2) buffer gglBuffer
{
	double v[];
} ggl;
layout (binding = 3) buffer igBuffer
{
	int v[];
} ig;
layout (binding = 4) buffer jgBuffer
{
	int v[];
} jg;
layout (binding = 5) buffer x0Buffer
{
	double v[cs];
} x0;
layout (binding = 6) buffer outBuffer
{
	double v[cs];
} o;

uniform int size;
uniform int maxItt;
uniform float eps;

const uint i = gl_LocalInvocationID.x;

shared double p[cs];
shared double r[cs];
shared double temp[cs];
shared double ap[cs];
shared double xTemp[cs];

shared double bb;
shared double rr;
shared double rr1;
shared double app;

void Stop()
{
	barrier();
	memoryBarrierShared();
}

void MultVecToA(double[cs] x, out double[cs] res)
{
	res[i] = x[i] * di.v[i];
	for (int j = ig.v[i]; j < ig.v[i + 1]; j++)
	{
		res[i] += ggl.v[j] * x[jg.v[j]];
		res[jg.v[j]] += ggl.v[j] * x[i];
	}
}

void VecMinVec(double[cs] x, double[cs] y, out double[cs] res)
{
	res[i] = x[i] - y[i];
}

void VecPlusVec(double[cs] x, double[cs] y, out double[cs] res)
{
	res[i] = x[i] + y[i];
}

void FindScalar(double[cs] x, double[cs] y, out double res)
{
	res += x[i] * y[i];
}

void VecMultByNumber(double[cs] x, double num, out double[cs] res)
{
	res[i] = x[i] * num;
}

void main(void)
{
	double alpha;
	double beta;

	MultVecToA(x0.v, temp);

	Stop();

	VecMinVec(b.v, temp, r);

	Stop();

	xTemp = x0.v;
//	FindScalar(b.v, b.v, bb);
	atomicAdd(qq, float(b.v[i] * b.v[i]));
	o.v[i] = bb;
	return;
	FindScalar(r, r, rr1);
	Stop();

	for (int k = 0; k < maxItt; k++)
	{
		rr = rr1;
		MultVecToA(p, ap);

		Stop();

		FindScalar(ap, p, app);

		Stop();

		alpha = rr / app;
		VecMultByNumber(p, alpha, temp);

		Stop();

		VecPlusVec(xTemp, temp, xTemp);
		VecMultByNumber(ap, alpha, temp);

		Stop();

		VecMinVec(r, temp, r);

		Stop();

		FindScalar(r, r, rr1);

		Stop();

		if ((rr1 / bb) < eps)
			break;

		beta = rr1 / rr;
		VecMultByNumber(p, beta, temp);

		Stop();
	}
	o.v = xTemp;
}
