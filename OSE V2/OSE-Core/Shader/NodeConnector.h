#pragma once
#include "ShaderNode.h"

namespace ose::shader
{
	class NodeConnector
	{
	public:
		NodeConnector(ShaderNode * in_node, std::string const & in_node_input,
						ShaderNode * out_node, std::string const & out_node_output);
		~NodeConnector();
		NodeConnector(NodeConnector &) = delete;
		NodeConnector & operator=(NodeConnector &) = delete;
		NodeConnector(NodeConnector &&);
		NodeConnector & operator=(NodeConnector &&);

		// Get the output node and the output name
		void GetOutput(ShaderNode ** node, std::string & name);

		// Get the input node and the input name
		void GetInput(ShaderNode ** node, std::string & name);

	private:

		// The output node and the name of the output point
		ShaderNode * out_node_;
		std::string out_node_output_;

		// The input node and the name of the input point
		ShaderNode * in_node_;
		std::string in_node_input_;
	};
}

