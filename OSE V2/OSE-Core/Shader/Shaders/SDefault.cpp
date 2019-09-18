#include "stdafx.h"
#include "SDefault.h"

namespace ose::shader
{
	SDefault::SDefault() : Shader("Default Built-in Shader")
	{
		// Add all the nodes required
		ShaderNode * color_node { AddNode<NConstColor>(glm::vec4{ 1, 0, 1, 1 }) };

		// Connect the nodes together
	}

	SDefault::~SDefault()
	{

	}
}
