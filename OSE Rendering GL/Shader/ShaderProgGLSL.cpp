#include "pch.h"
#include "ShaderProgGLSL.h"

namespace ose::shader
{
	ShaderProgGLSL::ShaderProgGLSL() : ShaderProg()
	{

	}

	ShaderProgGLSL::~ShaderProgGLSL()
	{

	}

	// Build a shader object from a shader graph
	void ShaderProgGLSL::BuildShaderGraph(ShaderGraph & shader_graph)
	{
		// TODO - Work backwards from output color node
		// Start code in the lowest level shader (fragment)
		// Whenever a node is encountered requiring the higher level, ...
		// ...begin that shader & bridge connection between the two shaders
	///	ProcessIncomingConnectors(shader_graph, shader_graph.GetOutputColorNode());
	}

	// Process the incoming connectors of node n
	void ShaderProgGLSL::ProcessIncomingConnectors(ShaderGraph & shader_graph, unowned_ptr<ShaderNode> n)
	{
		for(auto node : shader_graph.GetIncomingConnectors(shader_graph.GetOutputColorNode()))
		{
			
		}
	}
}
