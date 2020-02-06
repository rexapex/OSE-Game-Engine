#include "stdafx.h"
#include "ShaderGraphPBR3D.h"
#include "OSE-Core/Shader/Nodes/TextureNode.h"
#include "OSE-Core/Shader/Nodes/BRDFNode.h"

namespace ose
{
	ShaderGraphPBR3D::ShaderGraphPBR3D() : ShaderGraph("OSE PBR 3D Shader")
	{
		// Add all the nodes required
		unowned_ptr<ShaderNode> albedo_tex_node { AddNode<TextureNode>() };
		unowned_ptr<ShaderNode> brdf_node { AddNode<BRDFNode>() };

		// Connect the nodes together
		Connect(albedo_tex_node, "color", GetOutputColorNode(), "color");
	}

	ShaderGraphPBR3D::~ShaderGraphPBR3D()
	{

	}
}
