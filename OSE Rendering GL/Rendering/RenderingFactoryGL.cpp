#include "pch.h"
#include "RenderingFactoryGL.h"
#include "RenderingEngineGL.h"
#include "TextureGL.h"
#include "Shader/ShaderProgGLSL.h"

// TODO - Remove
#include "OSE-Core/Shader/Shaders/ShaderGraph2D.h"
#include "Shader/Shaders/Default2DShaderProgGLSL.h"
#include "OSE-Core/Shader/Shaders/ShaderGraph3D.h"
#include "Shader/Shaders/BRDFShaderProgGLSL.h"

namespace ose::rendering
{
	uptr<RenderingEngine> RenderingFactoryGL::NewRenderingEngine(int fbwidth, int fbheight, std::vector<char const *> const & extensions)
	{
		return ose::make_unique<RenderingEngineGL>(fbwidth, fbheight);
	}

	uptr<Texture> RenderingFactoryGL::NewTexture(std::string const & name, std::string const & path)
	{
		return ose::make_unique<TextureGL>(name, path);
	}

	uptr<ShaderProg> RenderingFactoryGL::NewShaderProg(uptr<ShaderGraph> shader_graph)
	{
		// TODO - Remove tests for specific shader graphs once shader graph implementation is finished
		ShaderGraph2D * prog { dynamic_cast<ShaderGraph2D *>(shader_graph.get()) };
		if(prog != nullptr)
			return ose::make_unique<ose::shader::Default2DShaderProgGLSL>();
		ShaderGraph3D * prog2 { dynamic_cast<ShaderGraph3D *>(shader_graph.get()) };
		if(prog2 != nullptr)
			return ose::make_unique<ose::shader::BRDFShaderProgGLSL>();
		return ose::make_unique<ose::shader::ShaderProgGLSL>(std::move(shader_graph));
	}
}
