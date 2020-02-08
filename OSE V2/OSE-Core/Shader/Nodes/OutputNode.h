#pragma once
#include "OSE-Core/Shader/ShaderNode.h"

namespace ose
{
	class OutputNode final : public ShaderNode
	{
	public:
		OutputNode() : ShaderNode()
		{
			inputs_.emplace("color", ShaderVarType{ EBaseVarType::FLOAT, 4 });
		}

		~OutputNode() {}
	};
}
