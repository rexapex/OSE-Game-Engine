#include "pch.h"
#include "RenderingFactoryGL.h"
#include "RenderingEngineGL.h"
#include "TextureGL.h"

namespace ose::rendering
{
	std::unique_ptr<RenderingEngine> RenderingFactoryGL::NewRenderingEngine()
	{
		return std::make_unique<RenderingEngineGL>();
	}

	std::unique_ptr<ose::resources::Texture> RenderingFactoryGL::NewTexture(std::string const & name, std::string const & path)
	{
		return std::make_unique<TextureGL>(name, path);
	}
}
