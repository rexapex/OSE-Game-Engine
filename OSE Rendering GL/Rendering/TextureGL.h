#pragma once
#include "OSE-Core/Resources/Texture.h"

namespace ose::rendering
{
	class TextureGL : public ose::resources::Texture
	{
	public:
		// Texture is an abstract class, the full class will contain render library specific data
		TextureGL(const std::string & name, const std::string & path) : resources::Texture(name, path) {}
	private:
		// default to 0, i.e. no texture
		uint32_t gl_tex_id_ { 0 };
	public:
		// get the OpenGL texture ID
		const uint32_t GetGlTexId() const { return gl_tex_id_; }

		// bind the texture ready for rendering
		void Bind() const override;

		// create the texture in GPU memory
		// IMPORTANT - cannot be called from constructor since the ResourceLoader is multithreaded but the rendering context is not
		void CreateTexture() override;

		// free the texture from GPU memory
		// IMPORTANT - cannot be called from destructor since the ResourceManager is multithreaded but the rendering context is not
		// IMPORTANT - failure to call this may result in GPU memory leaks
		void DestroyTexture() override;
	};
}
