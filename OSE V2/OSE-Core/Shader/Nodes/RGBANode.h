#pragma once
#include "OSE-Core/Shader/ShaderNode.h"
#include "OSE-Core/Shader/ShaderNodeMacros.h"

namespace ose
{
	class RGBANode final : public ShaderNode
	{
		SHADER_NODE(RGBANode, ST_FRAGMENT)

	public:
		RGBANode(glm::vec4 const & rgba) : ShaderNode(), rgba_(rgba) {}
		~RGBANode() {}

	private:
		glm::vec4 rgba_;
	};
}
