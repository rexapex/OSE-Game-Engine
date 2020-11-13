#pragma once

#include "Texture.h"

namespace ose
{
	class TextureLoader
	{
	public:
		TextureLoader(std::string const & project_path);
		virtual ~TextureLoader();
		//TextureLoader is NOT copyable
		TextureLoader(TextureLoader &) = delete;
		TextureLoader & operator=(TextureLoader &) = delete;
		//TextureLoader IS movable
		TextureLoader(TextureLoader &&) noexcept = default;
		TextureLoader & operator=(TextureLoader &&) noexcept = default;

		//loads the texture and sets the values of img_data, width and height
		//path is absolute and is guaranteed to exist
		virtual void LoadTexture(std::string const & path, IMGDATA * img_data, int32_t * width, int32_t * height, int32_t * channels) = 0;

		//free resources used by the texture (img_data)
		virtual void FreeTexture(IMGDATA img_data) = 0;
	private:
		std::string project_path_;
	};
}
