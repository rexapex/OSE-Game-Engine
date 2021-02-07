#pragma once
#include "OSE-Core/Rendering/RenderingFactory.h"

namespace ose
{
	class Texture;
	class RenderingEngine;
}

namespace ose::rendering
{
	class RenderingFactoryGL : public RenderingFactory
	{
	public:
		constexpr RenderingFactoryGL() : RenderingFactory() {}
		virtual ~RenderingFactoryGL() {}
		RenderingFactoryGL(RenderingFactoryGL &) = delete;
		RenderingFactoryGL & operator=(RenderingFactoryGL &) = delete;
		RenderingFactoryGL(RenderingFactoryGL &&) = default;
		RenderingFactoryGL & operator=(RenderingFactoryGL &&) = default;

		virtual uptr<RenderingEngine> NewRenderingEngine(WindowManager const & window_manager);
		virtual uptr<Texture> NewTexture(std::string const & name, std::string const & path);
		virtual uptr<ShaderProg> NewShaderProg(uptr<ShaderGraph> shader_graph);
	};
}