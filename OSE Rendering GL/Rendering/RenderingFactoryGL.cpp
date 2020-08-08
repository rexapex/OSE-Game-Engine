#include "pch.h"
#include "RenderingFactoryGL.h"
#include "RenderingEngineGL.h"
#include "TextureGL.h"
#include "Shader/ShaderProgGLSL.h"

namespace ose::rendering
{
	std::unique_ptr<RenderingEngine> RenderingFactoryGL::NewRenderingEngine(int fbwidth, int fbheight)
	{
		return std::make_unique<RenderingEngineGL>(fbwidth, fbheight);
	}

	std::unique_ptr<Texture> RenderingFactoryGL::NewTexture(std::string const & name, std::string const & path)
	{
		return std::make_unique<TextureGL>(name, path);
	}

	std::unique_ptr<ShaderProg> RenderingFactoryGL::NewShaderProg(std::unique_ptr<ShaderGraph> shader_graph)
	{
		return std::make_unique<ose::shader::ShaderProgGLSL>(std::move(shader_graph));
	}
}
