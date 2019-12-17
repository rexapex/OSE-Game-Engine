#pragma once
#include "OSE-Core/Types.h"
#include "TextureGL.h"
#include "ERenderObjectType.h"

namespace ose::rendering
{
	struct RenderObjectGL
	{
		ERenderObjectType type_;

		uint32_t vbo_;
		uint32_t vao_;

		GLenum render_primitive_ { GL_TRIANGLES };
		GLint first_ { 0 };
		GLint count_ { 0 };

		// TODO - Implement stride (or something similar) to determine which textures belong to same instance within render object
		std::vector<ose::unowned_ptr<TextureGL const>> textures_;

		// TODO - Experiment with handling transforms differently
		// TODO - Replace with pointer to glm::mat4 s.t. changes made to entity transform immediately have effect on render object
		std::vector<glm::mat4> transforms_;
	};
}
