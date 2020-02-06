#pragma once
#include "OSE-Core/Shader/ShaderNode.h"

namespace ose
{
	class RGBANode final : public ShaderNode
	{
	public:
		RGBANode(glm::vec4 const & rgba) : ShaderNode(), rgba_(rgba) {}
		~RGBANode() {}

	private:
		glm::vec4 rgba_;
	};
}
