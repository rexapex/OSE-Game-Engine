#include "pch.h"
#include "RenderingFactoryGL.h"
#include "RenderingEngineGL.h"
#include "TextureGL.h"
#include "Shader/ShaderProgGLSL.h"

// TODO - Remove
#include "OSE-Core/Shader/Shaders/ShaderGraph2D.h"
#include "Shader/Shaders/Default2DShaderProgGLSL.h"

namespace ose::rendering
{
	uptr<RenderingEngine> RenderingFactoryGL::NewRenderingEngine(int fbwidth, int fbheight)
	{
		return ose::make_unique<RenderingEngineGL>(fbwidth, fbheight);
	}

	uptr<Texture> RenderingFactoryGL::NewTexture(std::string const & name, std::string const & path)
	{
		return ose::make_unique<TextureGL>(name, path);
	}

	uptr<ShaderProg> RenderingFactoryGL::NewShaderProg(uptr<ShaderGraph> shader_graph)
	{
		ShaderGraph2D * prog { dynamic_cast<ShaderGraph2D *>(shader_graph.get()) };
		if(prog != nullptr)
			return ose::make_unique<ose::shader::Default2DShaderProgGLSL>();
		return ose::make_unique<ose::shader::ShaderProgGLSL>(std::move(shader_graph));
	}
}
