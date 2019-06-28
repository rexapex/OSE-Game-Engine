#pragma once
#include "ShaderNode.h"

namespace ose::shader
{
	class NodeConnector
	{
	public:
		NodeConnector(const ShaderNode &, const ShaderNode &);
		~NodeConnector();

	private:
		const ShaderNode & in_node_;
		const ShaderNode & out_node_;
	};
}

