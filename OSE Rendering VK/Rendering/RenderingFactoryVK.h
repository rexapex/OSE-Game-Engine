#pragma once
#include "OSE-Core/Rendering/RenderingFactory.h"

namespace ose
{
	class Texture;
	class RenderingEngine;
}

namespace ose::rendering
{
	class RenderingFactoryVK : public RenderingFactory
	{
	public:
		constexpr RenderingFactoryVK() : RenderingFactory() {}
		virtual ~RenderingFactoryVK() {}
		RenderingFactoryVK(RenderingFactoryVK &) = delete;
		RenderingFactoryVK & operator=(RenderingFactoryVK &) = delete;
		RenderingFactoryVK(RenderingFactoryVK &&) = default;
		RenderingFactoryVK & operator=(RenderingFactoryVK &&) = default;

		virtual uptr<RenderingEngine> NewRenderingEngine(int fbwidth, int fbheight, std::vector<char const *> const & extensions);
		virtual uptr<Texture> NewTexture(std::string const & name, std::string const & path);
		virtual uptr<ShaderProg> NewShaderProg(uptr<ShaderGraph> shader_graph);
	};
}