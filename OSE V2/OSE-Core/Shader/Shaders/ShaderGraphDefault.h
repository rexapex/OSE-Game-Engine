#pragma once
#include "OSE-Core/Types.h"
#include "OSE-Core/Shader/ShaderGraph.h"
#include "OSE-Core/Shader/Nodes/NConstColor.h"

namespace ose::shader
{
	class ShaderGraphDefault : public ShaderGraph
	{
	public:
		ShaderGraphDefault();
		~ShaderGraphDefault();
	};
}
