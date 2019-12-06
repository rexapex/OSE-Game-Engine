#include "pch.h"
#include "TextureLoaderSOIL.h"
#include <SOIL.h>

namespace ose::resources
{
	TextureLoaderSOIL::TextureLoaderSOIL(const std::string & project_path) : TextureLoader(project_path) {}

	TextureLoaderSOIL::~TextureLoaderSOIL() {}

	// loads the texture and sets the values of img_data, width and height
	// path is absolute and is guaranteed to exist
	void TextureLoaderSOIL::LoadTexture(const std::string & path, IMGDATA * img_data, int32_t * width, int32_t * height)
	{
		*img_data = SOIL_load_image(path.c_str(), width, height, 0, SOIL_LOAD_AUTO);
	}

	// free resources used by the texture (img_data)
	void TextureLoaderSOIL::FreeTexture(IMGDATA img_data)
	{
		SOIL_free_image_data(img_data);
	}
}
