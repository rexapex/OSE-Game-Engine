#include "stdafx.h"
#include "NodeConnector.h"

namespace ose
{
	NodeConnector::NodeConnector(ShaderNode * in_node, std::string const & in_node_input,
								ShaderNode * out_node, std::string const & out_node_output)
		: in_node_(in_node), out_node_(out_node), in_node_input_(in_node_input), out_node_output_(out_node_output)
	{

	}

	NodeConnector::~NodeConnector()
	{

	}

	NodeConnector::NodeConnector(NodeConnector && other)
	{
		in_node_ = std::move(other.in_node_);
		out_node_ = std::move(other.out_node_);
		in_node_input_ = std::move(other.in_node_input_);
		out_node_output_ = std::move(other.out_node_output_);
	}

	NodeConnector & NodeConnector::operator=(NodeConnector && other)
	{
		in_node_ = std::move(other.in_node_);
		out_node_ = std::move(other.out_node_);
		in_node_input_ = std::move(other.in_node_input_);
		out_node_output_ = std::move(other.out_node_output_);
		return *this;
	}

	// Get the output node and the output name point
	void NodeConnector::GetOutput(ShaderNode ** node, std::string & name)
	{
		*node = out_node_;
		name = out_node_output_;
	}

	// Get the input node and the input name point
	void NodeConnector::GetInput(ShaderNode ** node, std::string & name)
	{
		*node = in_node_;
		name = in_node_input_;
	}
}
