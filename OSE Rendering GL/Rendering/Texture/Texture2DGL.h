#pragma once

namespace ose::rendering
{
	class Texture2DGL
	{
	public:
		// Texture is an abstract class, the full class will contain render library specific data
		Texture2DGL() {}

		// Create the texture in GPU memory
		void Create(int32_t width, int32_t height, int32_t channels, unsigned char const * img_data);

		// Free the texture from GPU memory
		// IMPORTANT - failure to call this may result in GPU memory leaks
		void Destroy();

		// Upload data to the texture
		// Assumes (x, y, w, h) sub rect is within the bounds of the texture
		void UploadData(int32_t x, int32_t y, int32_t w, int32_t h, int32_t channels, unsigned char const * img_data);

		// TODO - Apply texture meta file

		// Get the OpenGL texture ID
		uint32_t GetGlTexId() const { return gl_tex_id_; }

	private:
		// Default to 0, i.e. no texture
		uint32_t gl_tex_id_ { 0 };
	};
}
