#pragma once
#include "OSE-Core/Shader/ShaderNode.h"

namespace ose
{
	class NConstColor : public ShaderNode
	{
	public:
		NConstColor();
		NConstColor(glm::vec4 const & color);
		~NConstColor();

		void SetColor(glm::vec4 const & color) { color_ = color; }
		glm::vec4 const & GetColor() const { return color_; }

	private:
		glm::vec4 color_ { 1, 1, 1, 1 };
	};
}

