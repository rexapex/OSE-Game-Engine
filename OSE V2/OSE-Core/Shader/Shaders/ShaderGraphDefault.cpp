#include "stdafx.h"
#include "ShaderGraphDefault.h"

namespace ose
{
	ShaderGraphDefault::ShaderGraphDefault() : ShaderGraph("Default Built-in Shader")
	{
		// Add all the nodes required
		ose::unowned_ptr<ShaderNode> color_node { AddNode<NConstColor>(glm::vec4{ 1, 0, 1, 1 }) };

		// Connect the nodes together
		Connect(color_node, "color", GetOutputColorNode(), "color");
	}

	ShaderGraphDefault::~ShaderGraphDefault()
	{

	}
}
