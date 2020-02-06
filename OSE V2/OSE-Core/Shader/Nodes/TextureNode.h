#pragma once
#include "OSE-Core/Shader/ShaderNode.h"

namespace ose
{
	class TextureNode final : public ShaderNode
	{
	public:
		TextureNode() : ShaderNode()
		{
			outputs_.emplace("color", ShaderVarType{ EBaseVarType::FLOAT, 4 });
		}

		~TextureNode() {}
	};
}
