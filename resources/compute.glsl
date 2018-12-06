#version 450 core
#extension GL_NV_shader_atomic_float : enable

// TODO: CONFIGURE THIS CONST
const int cs = 100;

layout (local_size_x = cs) in;

layout (binding = 0) buffer bBuffer
{
	float v[cs];
} b;
layout (binding = 1) buffer diBuffer
{
	float v[cs];
} di;
layout (binding = 2) buffer gglBuffer
{
	float v[];
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
	float v[cs];
} x0;
layout (binding = 6) buffer outBuffer
{
	float v[cs];
} o;

uniform int size;
uniform int maxItt;
uniform float eps;

const uint i = gl_LocalInvocationID.x;

shared float p[cs];
shared float r[cs];
shared float temp[cs];
shared float ap[cs];
shared float xTemp[cs];

shared float bb = 0.f;
shared float rr = 0.f;
shared float rr1 = 0.f;
shared float app = 0.f;

void Stop()
{
	barrier();
	memoryBarrierShared();
}

void main(void)
{
	float alpha = 0;
	float beta = 0;

	temp[i] = x0.v[i] * di.v[i];
	for (int j = ig.v[i]; j < ig.v[i + 1]; j++)
	{
		temp[i] += ggl.v[j] * x0.v[jg.v[j]];
		atomicAdd(temp[jg.v[j]], ggl.v[j] * x0.v[i]);
	}

	Stop();

	r[i] = b.v[i] - temp[i];

	Stop();

	xTemp = x0.v;
	p = r;

	atomicAdd(bb, b.v[i] * b.v[i]);
	atomicAdd(rr1, r[i] * r[i]);

	Stop();

	for (int k = 0; k < maxItt; k++)
	{
		rr = rr1;
		temp[i] = p[i] * di.v[i];
		for (int j = ig.v[i]; j < ig.v[i + 1]; j++)
		{
			temp[i] += ggl.v[j] * p[jg.v[j]];
			atomicAdd(temp[jg.v[j]], ggl.v[j] * p[i]);
		}

		Stop();

		ap = temp;
		app = 0.f;
		atomicAdd(app, ap[i] * p[i]);

		Stop();

		alpha = rr / app;
		temp[i] = alpha * p[i];

		Stop();

		atomicAdd(xTemp[i], temp[i]);
		temp[i] = alpha * ap[i];

		Stop();

		atomicAdd(r[i], -temp[i]);

		Stop();
		rr1 = 0.f;
		atomicAdd(rr1, r[i] * r[i]);

		Stop();

		if ((rr1 / bb) < eps)
			break;

		beta = rr1 / rr;
		temp[i] = beta * p[i];

		Stop();

		p[i] = r[i] + temp[i];

		Stop();
	}
	o.v = xTemp;
}
