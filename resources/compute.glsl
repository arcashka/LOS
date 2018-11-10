#version 450 core

layout (local_size_x = 1024) in;

shared float shared_data[gl_WorkGroupSize.x * 2];

layout (binding = 0, r32f) readonly uniform image2D input_image;
layout (binding = 1, r32f) writeonly uniform image2D output_image;

void main(void)
{
	uint id = gl_LocalInvocationID.x;
	uint rd_id;
	uint wr_id;
	uint mask;
	ivec2 P = ivec2(id * 2, gl_WorkGroupID.x);

	const uint steps = uint(log2(gl_WorkGroupSize.x)) + 1;
	uint step = 0;

	shared_data[id * 2] = imageLoad(input_image, P).r;
	shared_data[id * 2 + 1] = imageLoad(input_image, P + ivec2(1, 0)).r;

	barrier();
	memoryBarrierShared();

	for (step = 0; step < steps; step++)
	{
		mask = (1 << step) - 1;
		rd_id = ((id >> step) << (step + 1)) + mask;
		wr_id = rd_id + 1 + (id & mask);
		shared_data[wr_id] += shared_data[rd_id];
		barrier();
		memoryBarrierShared();
	}

	imageStore(output_image, P.yx, vec4(shared_data[id * 2]));
	imageStore(output_image, P.yx + ivec2(0, 1), vec4(shared_data[id * 2 + 1]));
}
