#pragma once
#include "OSE-Core/Shader/ShaderNode.h"

namespace ose
{
	class BRDFNode final : public ShaderNode
	{
	public:
		BRDFNode() : ShaderNode()
		{
			inputs_.emplace("color", ShaderVarType{ EBaseVarType::FLOAT, 4 });
			outputs_.emplace("color", ShaderVarType{ EBaseVarType::FLOAT, 4 });
		}

		~BRDFNode() {}
	};
}
