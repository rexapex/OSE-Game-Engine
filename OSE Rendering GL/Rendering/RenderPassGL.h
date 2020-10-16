#pragma once
#include "ShaderGroupGL.h"

namespace ose::rendering
{
	struct RenderPassGL
	{
		GLuint fbo_				{ 0 };
		GLbitfield clear_		{ GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT };
		std::vector<ShaderGroupGL> shader_groups_;
	};
}
