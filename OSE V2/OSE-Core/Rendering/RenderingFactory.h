#pragma once

namespace ose
{
	class Texture;
	class RenderingEngine;
	class ShaderProg;
	class ShaderGraph;

	class RenderingFactory
	{
	public:
		constexpr RenderingFactory() {}
		virtual ~RenderingFactory() {}
		RenderingFactory(RenderingFactory &) = delete;
		RenderingFactory & operator=(RenderingFactory &) = delete;
		RenderingFactory(RenderingFactory &&) = default;
		RenderingFactory & operator=(RenderingFactory &&) = default;

		virtual std::unique_ptr<RenderingEngine> NewRenderingEngine(int fbwidth, int fbheight) = 0;
		virtual std::unique_ptr<Texture> NewTexture(std::string const & name, std::string const & path) = 0;
		virtual std::unique_ptr<ShaderProg> NewShaderProg(std::unique_ptr<ShaderGraph> shader_graph) = 0;
	};
}
