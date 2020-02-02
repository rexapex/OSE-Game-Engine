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

		virtual std::unique_ptr<RenderingEngine> NewRenderingEngine(int fbwidth, int fbheight);
		virtual std::unique_ptr<Texture> NewTexture(std::string const & name, std::string const & path);
	};
}