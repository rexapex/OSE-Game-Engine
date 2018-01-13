#pragma once

#include "stdafx.h"
#include "Texture.h"

namespace origami_sheep_engine
{
	class TextureLoader
	{
	public:
		TextureLoader(const std::string & project_path);
		~TextureLoader();
		//TextureLoader is NOT copyable
		TextureLoader(TextureLoader &) = delete;
		TextureLoader & operator=(TextureLoader &) = delete;
		//TextureLoader IS movable
		TextureLoader(TextureLoader &&) noexcept = default;
		TextureLoader & operator=(TextureLoader &&) noexcept = default;
	private:
		std::string project_path_;

		//loads the texture and sets the values of img_data, width and height
		//path is absolute and is guaranteed to exist
		virtual void loadTexture(const std::string & path, IMGDATA img_data, uint32_t * width, uint32_t * height) = 0;

		//free resources used by the texture (img_data)
		virtual void freeTexture(const Texture & tex) = 0;
	};
}
