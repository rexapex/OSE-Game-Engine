#pragma once
#include "ShaderGraph.h"

namespace ose
{
	class ShaderProg
	{
	public:
		ShaderProg(std::unique_ptr<ShaderGraph> shader_graph) : shader_graph_(std::move(shader_graph)) {}
		virtual ~ShaderProg() {}

		// Build a gpu shader object from a shader graph
		virtual void CreateShaderProg() = 0;

		// Destroy the gpu shader object
		virtual void DestroyShaderProg() = 0;

	protected:

		// The shader graph used to build the shader program (can be nullptr if program is defined by other means than shader graph)
		std::unique_ptr<ShaderGraph> shader_graph_;
	};
}

