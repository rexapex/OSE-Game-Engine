#pragma once
#include "OSE-Core/Shader/EShaderType.h"

namespace ose::shader
{
	struct ShaderLayer
	{
		int type_ { ST_UNKNOWN };
		std::vector<unowned_ptr<ShaderNode>> nodes_;
	};
}
