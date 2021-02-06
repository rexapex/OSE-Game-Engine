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

		virtual uptr<RenderingEngine> NewRenderingEngine(int fbwidth, int fbheight, std::vector<char const *> const & extensions) = 0;
		virtual uptr<Texture> NewTexture(std::string const & name, std::string const & path) = 0;
		virtual uptr<ShaderProg> NewShaderProg(uptr<ShaderGraph> shader_graph) = 0;
	};
}
