#include "pch.h"
#include "TextureLoaderFactorySOIL.h"
#include "TextureLoaderSOIL.h"

namespace ose::resources
{
	uptr<TextureLoader> TextureLoaderFactorySOIL::NewTextureLoader(std::string const & project_path)
	{
		return ose::make_unique<TextureLoaderSOIL>(project_path);
	}
}
