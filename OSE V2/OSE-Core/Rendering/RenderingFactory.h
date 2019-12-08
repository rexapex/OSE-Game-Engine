#pragma once

namespace ose::resources
{
	class Texture;
}

namespace ose::rendering
{
	class RenderingEngine;

	class RenderingFactory
	{
	public:
		constexpr RenderingFactory() {}
		virtual ~RenderingFactory() {}
		RenderingFactory(RenderingFactory &) = delete;
		RenderingFactory & operator=(RenderingFactory &) = delete;
		RenderingFactory(RenderingFactory &&) = default;
		RenderingFactory & operator=(RenderingFactory &&) = default;

		virtual std::unique_ptr<RenderingEngine> NewRenderingEngine() = 0;
		virtual std::unique_ptr<ose::resources::Texture> NewTexture(std::string const & name, std::string const & path) = 0;
	};
}
