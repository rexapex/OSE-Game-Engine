#pragma once
#include "ShaderGroupGL.h"

namespace ose::rendering
{
	struct RenderPassGL
	{
		/*GLuint fbo_;
		GLuint fbo_width_;
		GLuint fbo_height_;
		GLenum clear_;*/
		std::vector<ShaderGroupGL> shader_groups_;
	};
}
