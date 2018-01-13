#pragma once

//currently, the values correspond to OpenGL 4.4 Texture Filtering values
enum class ETextureFilterMode
{
	NEAREST,
	LINEAR,
	NEAREST_MIPMAP_NEAREST,
	LINEAR_MIPMAP_NEAREST,
	NEAREST_MIPMAP_LINEAR,
	LINEAR_MIPMAP_LINEAR
};
