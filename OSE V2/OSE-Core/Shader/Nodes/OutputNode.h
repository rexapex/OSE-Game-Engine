#pragma once
#include "OSE-Core/Shader/ShaderNode.h"
#include "OSE-Core/Shader/ShaderNodeMacros.h"

namespace ose
{
	class OutputNode final : public ShaderNode
	{
		SHADER_NODE(OutputNode, ST_FRAGMENT)

	public:
		OutputNode() : ShaderNode()
		{
			inputs_.emplace("color", ShaderVarType{ EBaseVarType::FLOAT, 4 });
		}

		~OutputNode() {}
	};
}
