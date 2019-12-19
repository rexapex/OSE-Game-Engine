#pragma once
#include "OSE-Core/Types.h"
#include "TextureGL.h"
#include "ERenderObjectType.h"

namespace ose::rendering
{
	struct RenderObjectGL
	{
		ERenderObjectType type_;

		GLuint vbo_;
		GLuint vao_;

		GLenum render_primitive_ { GL_TRIANGLES };
		GLint first_ { 0 };
		GLint count_ { 0 };

		// TODO - Implement stride (or something similar) to determine which textures belong to same instance within render object
		std::vector<GLuint> textures_;

		// TODO - Experiment with handling transforms differently
		// TODO - Replace with pointer to glm::mat4 s.t. changes made to entity transform immediately have effect on render object
		// TODO - Also consider using observer pattern to notify render object of transform changes
		std::vector<glm::mat4> transforms_;

		RenderObjectGL(ERenderObjectType type, GLuint vbo, GLuint vao, GLenum render_primitive, GLint first,
				GLint count, std::initializer_list<GLuint> textures, std::initializer_list<glm::mat4> transforms)
			: type_(type), vbo_(vbo), vao_(vao), render_primitive_(render_primitive),
				first_(first), count_(count), textures_(textures), transforms_(transforms) {}
	};
}
