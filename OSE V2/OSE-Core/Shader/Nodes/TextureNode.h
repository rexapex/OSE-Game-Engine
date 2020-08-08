#pragma once
#include "OSE-Core/Shader/ShaderNode.h"
#include "OSE-Core/Shader/ShaderNodeMacros.h"

namespace ose
{
	class TextureNode final : public ShaderNode
	{
		SHADER_NODE(TextureNode, ST_FRAGMENT)

	public:
		TextureNode() : ShaderNode()
		{
			outputs_.emplace("color", ShaderVarType{ EBaseVarType::FLOAT, 4 });
		}

		~TextureNode() {}
	};
}
