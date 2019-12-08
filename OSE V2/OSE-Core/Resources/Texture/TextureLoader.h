#pragma once

#include "Texture.h"

namespace ose::resources
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

		//loads the texture and sets the values of img_data, width and height
		//path is absolute and is guaranteed to exist
		virtual void LoadTexture(const std::string & path, IMGDATA * img_data, int32_t * width, int32_t * height) = 0;

		//free resources used by the texture (img_data)
		virtual void FreeTexture(IMGDATA img_data) = 0;
	private:
		std::string project_path_;
	};
}
