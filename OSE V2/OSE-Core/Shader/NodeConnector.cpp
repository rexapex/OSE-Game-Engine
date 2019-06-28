#include "stdafx.h"
#include "NodeConnector.h"

namespace ose::shader
{
	NodeConnector::NodeConnector(const ShaderNode & in_node, const ShaderNode & out_node) : in_node_(in_node), out_node_(out_node)
	{

	}

	NodeConnector::~NodeConnector()
	{

	}
}
