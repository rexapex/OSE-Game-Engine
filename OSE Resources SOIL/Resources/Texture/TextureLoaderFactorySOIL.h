#pragma once
#include "OSE-Core/Resources/Texture/TextureLoaderFactory.h"

namespace ose
{
	class TextureLoader;
}

namespace ose::resources
{
	class TextureLoaderFactorySOIL : public TextureLoaderFactory
	{
	public:
		constexpr TextureLoaderFactorySOIL() {}
		virtual ~TextureLoaderFactorySOIL() {}
		TextureLoaderFactorySOIL(TextureLoaderFactorySOIL &) = delete;
		TextureLoaderFactorySOIL & operator=(TextureLoaderFactorySOIL &) = delete;
		TextureLoaderFactorySOIL(TextureLoaderFactorySOIL &&) = default;
		TextureLoaderFactorySOIL & operator=(TextureLoaderFactorySOIL &&) = default;

		virtual std::unique_ptr<TextureLoader> NewTextureLoader(std::string const & project_path);
	};
}
