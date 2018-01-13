#pragma once

//currently, the values correspond to OpenGL 4.4 Edge Value Sampling values
enum class ETextureEdgeSamplingMode
{
	//examples taken from: https://www.khronos.org/opengl/wiki/Sampler_Object
	REPEAT,					//the texture coordinate wraps around the texture. So a texture coordinate of -0.2 becomes the equivalent of 0.8
	MIRRORED_REPEAT,		//the texture coordinate wraps around like a mirror. -0.2 becomes 0.2, -1.2 becomes 0.8
	CLAMP_TO_EDGE,			//the texture coordinate is clamped to the [0, 1] range
	CLAMP_TO_BORDER,		//the texture coordinate is clamped to the [0, 1] range, but the edge texels are blended with a constant border color
	MIRROR_CLAMP_TO_EDGE	//the texture coordinates are clamped to the [-1, 1] range, but the negative coordinates are mirrors of the positive. This effectively makes the texture twice as big through mirroring, but clamps to the edge beyond that
};
