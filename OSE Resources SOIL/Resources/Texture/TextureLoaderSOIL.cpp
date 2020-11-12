#include "pch.h"
#include "TextureLoaderSOIL.h"
#include <SOIL.h>

namespace ose::resources
{
	TextureLoaderSOIL::TextureLoaderSOIL(std::string const & project_path) : TextureLoader(project_path) {}

	TextureLoaderSOIL::~TextureLoaderSOIL() {}

	// loads the texture and sets the values of img_data, width and height
	// path is absolute and is guaranteed to exist
	void TextureLoaderSOIL::LoadTexture(std::string const & path, IMGDATA * img_data, int32_t * width, int32_t * height, int32_t * channels)
	{
		*img_data = SOIL_load_image(path.c_str(), width, height, channels, SOIL_LOAD_AUTO);
		if(*img_data == NULL)
			LOG_ERROR("Failed to load texture", path, "- Check bit depth is 24-bit for RGB or 32-bit for RGBA");
	}

	// free resources used by the texture (img_data)
	void TextureLoaderSOIL::FreeTexture(IMGDATA img_data)
	{
		SOIL_free_image_data(img_data);
	}
}
