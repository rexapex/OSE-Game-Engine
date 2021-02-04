#include "pch.h"
#include "RenderingFactoryVK.h"
#include "RenderingEngineVK.h"

// TODO - Remove
#include "OSE-Core/Resources/Texture/Texture.h"
#include "OSE-Core/Shader/ShaderProg.h"
#include "OSE-Core/Shader/Shaders/ShaderGraph2D.h"
#include "OSE-Core/Shader/Shaders/ShaderGraph3D.h"

namespace ose::rendering
{
	uptr<RenderingEngine> RenderingFactoryVK::NewRenderingEngine(int fbwidth, int fbheight)
	{
		return ose::make_unique<RenderingEngineVK>(fbwidth, fbheight);
	}

	uptr<Texture> RenderingFactoryVK::NewTexture(std::string const & name, std::string const & path)
	{
		return nullptr;
	}

	uptr<ShaderProg> RenderingFactoryVK::NewShaderProg(uptr<ShaderGraph> shader_graph)
	{
		return nullptr;
	}
}
