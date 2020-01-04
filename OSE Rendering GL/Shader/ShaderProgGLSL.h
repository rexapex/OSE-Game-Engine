#pragma once
#include "OSE-Core/Shader/ShaderProg.h"
#include "OSE-Core/Shader/ShaderNode.h"
#include "VertShaderGLSL.h"
#include "FragShaderGLSL.h"

namespace ose::shader
{
	class ShaderProgGLSL : public ShaderProg
	{
	public:
		ShaderProgGLSL();
		virtual ~ShaderProgGLSL();

		// Build a shader object from a shader graph
		virtual void BuildShaderGraph(ShaderGraph & shader_graph);

		// Get the shader program id
		uint32_t GetShaderProgId() const { return shader_prog_; }

	private:
		// GLSL shader program is composed of a vertex shader and a fragment shader
		// TODO - Expand functionality to include other shader types
		// TODO - Possibly only need one class, ShaderGLSL rather than one for each shader type
		VertShaderGLSL vert_shader_;
		FragShaderGLSL frag_shader_;

		// OpenGL shader program id
		uint32_t shader_prog_ { 0 };

		// Process the incoming connectors of node n
		void ProcessIncomingConnectors(ShaderGraph & shader_graph, unowned_ptr<ShaderNode> n);
	};
}

