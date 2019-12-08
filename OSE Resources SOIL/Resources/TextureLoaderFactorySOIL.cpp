#include "pch.h"
#include "TextureLoaderFactorySOIL.h"
#include "TextureLoaderSOIL.h"

namespace ose::resources
{
	std::unique_ptr<TextureLoader> TextureLoaderFactorySOIL::NewTextureLoader(std::string const & project_path)
	{
		return std::make_unique<TextureLoaderSOIL>(project_path);
	}
}
