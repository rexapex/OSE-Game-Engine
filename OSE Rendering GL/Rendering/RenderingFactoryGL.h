#pragma once
#include "OSE-Core/Rendering/RenderingFactory.h"

namespace ose::resources
{
	class Texture;
}

namespace ose::rendering
{
	class RenderingEngine;

	class RenderingFactoryGL : public RenderingFactory
	{
	public:
		constexpr RenderingFactoryGL() : RenderingFactory() {}
		virtual ~RenderingFactoryGL() {}
		RenderingFactoryGL(RenderingFactoryGL &) = delete;
		RenderingFactoryGL & operator=(RenderingFactoryGL &) = delete;
		RenderingFactoryGL(RenderingFactoryGL &&) = default;
		RenderingFactoryGL & operator=(RenderingFactoryGL &&) = default;

		virtual std::unique_ptr<RenderingEngine> NewRenderingEngine();
		virtual std::unique_ptr<ose::resources::Texture> NewTexture(std::string const & name, std::string const & path);
	};
}