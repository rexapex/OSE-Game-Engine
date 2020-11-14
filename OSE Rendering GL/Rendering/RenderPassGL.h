#pragma once
#include "ShaderGroupGL.h"

namespace ose::rendering
{
	struct RenderPassGL
	{
		GLuint fbo_				{ 0 };
		bool clear_				{ false };
		GLbitfield clear_mode_  { 0 };

		bool enable_depth_test_ { false };
		GLenum depth_func_		{ GL_LEQUAL };

		std::vector<ShaderGroupGL> shader_groups_;
	};
}
