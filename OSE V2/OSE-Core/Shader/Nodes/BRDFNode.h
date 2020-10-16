#pragma once
#include "OSE-Core/Shader/ShaderNode.h"
#include "OSE-Core/Shader/ShaderNodeMacros.h"

namespace ose
{
	class BRDFNode final : public ShaderNode
	{
		SHADER_NODE(BRDFNode, ST_FRAGMENT)

	public:
		BRDFNode() : ShaderNode()
		{
			inputs_.emplace("color", ShaderVarType{ EBaseVarType::FLOAT, 4 });
			outputs_.emplace("color", ShaderVarType{ EBaseVarType::FLOAT, 4 });
		}

		~BRDFNode() {}
	};
}
