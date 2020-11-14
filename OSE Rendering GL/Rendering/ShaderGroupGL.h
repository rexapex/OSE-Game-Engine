#pragma once
#include "RenderObjectGL.h"

namespace ose::rendering
{
	struct ShaderGroupGL
	{
		GLuint shader_prog_		{ 0 };

		bool enable_blend_ { false };
		GLenum blend_fac_  { GL_SRC_ALPHA };
		GLenum blend_func_ { GL_ONE_MINUS_SRC_ALPHA };

		std::vector<RenderObjectGL> render_objects_;
	};
}
