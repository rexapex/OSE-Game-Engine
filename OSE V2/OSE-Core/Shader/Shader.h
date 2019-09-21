#pragma once
#include "ShaderGraph.h"

namespace ose::shader
{
	class Shader
	{
	public:
		Shader() {}
		virtual ~Shader() {}

		// Build a shader object from a shader graph
		virtual void BuildShaderGraph(ShaderGraph & shader_graph) = 0;
	};
}

