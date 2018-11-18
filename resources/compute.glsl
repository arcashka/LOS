#version 450 core

layout (local_size_x = 100) in;
layout (binding = 0) buffer bBuffer
{
	double member[];
} b;
layout (binding = 1) buffer diBuffer
{
	double member[];
} di;
layout (binding = 2) buffer gglBuffer
{
	double member[];
} ggl;
layout (binding = 3) buffer igBuffer
{
	int member[];
} ig;
layout (binding = 4) buffer jgBuffer
{
	int member[];
} jg;
layout (binding = 5) buffer outBuffer
{
	double member[];
} o;

void main(void)
{
	uint i = gl_LocalInvocationID.x;
	o.member[i] = i;//di.member[i] + b.member[i];
	barrier();
}
