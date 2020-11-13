#pragma once
#include "OSE-Core/Resources/Texture/TextureLoader.h"

namespace ose::resources
{
	class TextureLoaderSOIL : public TextureLoader
	{
	public:
		TextureLoaderSOIL(std::string const & project_path);
		~TextureLoaderSOIL();
		//TextureLoader is NOT copyable
		TextureLoaderSOIL(TextureLoaderSOIL &) = delete;
		TextureLoaderSOIL & operator=(TextureLoaderSOIL &) = delete;
		//TextureLoader IS movable
		TextureLoaderSOIL(TextureLoaderSOIL &&) noexcept = default;
		TextureLoaderSOIL & operator=(TextureLoaderSOIL &&) noexcept = default;

		//loads the texture and sets the values of img_data, width and height
		//path is absolute and is guaranteed to exist
		virtual void LoadTexture(std::string const & path, IMGDATA * img_data, int32_t * width, int32_t * height, int32_t * channels);

		//free resources used by the texture (img_data)
		virtual void FreeTexture(IMGDATA img_data);
	};
}

