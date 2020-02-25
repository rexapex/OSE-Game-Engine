#pragma once

namespace ose
{
	class TextureLoader;

	class TextureLoaderFactory
	{
	public:
		constexpr TextureLoaderFactory() {}
		virtual ~TextureLoaderFactory() {}
		TextureLoaderFactory(TextureLoaderFactory &) = delete;
		TextureLoaderFactory & operator=(TextureLoaderFactory &) = delete;
		TextureLoaderFactory(TextureLoaderFactory &&) = default;
		TextureLoaderFactory & operator=(TextureLoaderFactory &&) = default;

		virtual std::unique_ptr<TextureLoader> NewTextureLoader(std::string const & project_path) = 0;
	};
}
