#pragma once
#include "ShaderGraph.h"

namespace ose::shader
{
	class ShaderProg
	{
	public:
		ShaderProg() {}
		virtual ~ShaderProg() {}

		// Build a shader object from a shader graph
		virtual void BuildShaderGraph(ShaderGraph & shader_graph) = 0;
	};
}

