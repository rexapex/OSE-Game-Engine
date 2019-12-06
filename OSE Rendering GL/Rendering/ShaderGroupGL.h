#pragma once
#include "RenderObjectGL.h"

namespace ose::rendering
{
	struct ShaderGroupGL
	{
		uint32_t shader_prog_;
		std::vector<RenderObjectGL> render_objects_;
	};
}
