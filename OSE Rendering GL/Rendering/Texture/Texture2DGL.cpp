#include "pch.h"
#include "Texture2DGL.h"

namespace ose::rendering
{
	// Create the texture in GPU memory
	void Texture2DGL::Create(int32_t width, int32_t height, int32_t channels, unsigned char const * img_data)
	{
		// First, make sure any existing texture is freed
		Destroy();

		// Then, create the new OpenGL texture
		glGenTextures(1, &gl_tex_id_);

		// Set the contents of the texture
		glBindTexture(GL_TEXTURE_2D, gl_tex_id_);
		if(channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data);
		else if(channels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
	}

	// Free the texture from GPU memory
	// IMPORTANT - failure to call this may result in GPU memory leaks
	void Texture2DGL::Destroy()
	{
		// Check that the gl_tex_id_ is valid
		if(gl_tex_id_ != 0)
		{
			// If it is valid, free it then set the variable to 0
			glDeleteTextures(1, &gl_tex_id_);
			gl_tex_id_ = 0;
		}
	}

	// Upload data to the texture
	// If the xywh is not within the bounds of the texture, an exception is thrown
	void Texture2DGL::UploadData(int32_t x, int32_t y, int32_t w, int32_t h, int32_t channels, unsigned char const * img_data)
	{
		glBindTexture(GL_TEXTURE_2D, gl_tex_id_);
		if(channels == 4)
			glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, img_data);
		else if(channels == 3)
			glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE, img_data);
	}
}

