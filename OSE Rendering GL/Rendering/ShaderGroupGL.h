#pragma once
#include "RenderObjectGL.h"

namespace ose::rendering
{
	struct ShaderGroupGL
	{
		GLuint shader_prog_		{ 0 };

		std::vector<RenderObjectGL> render_objects_;
	};
}
