#include "stdafx.h"
#include "ShaderGraph.h"

namespace ose::shader
{
	ShaderGraph::ShaderGraph(const std::string & name) : name_(name)
	{
		// Every shader graph contains a single color output node, the first node in the graph
		AddNode<ShaderNode>();
	}

	ShaderGraph::~ShaderGraph()
	{

	}

	// Remove a shader node from the shader
	// Returns true iff the node exists and is removed successfully
	bool ShaderGraph::RemoveNode(ose::unowned_ptr<ShaderNode> node)
	{
		// No node can be removed if there are no nodes therefore return false
		// Nullptr cannot be in list of nodes_ therefore exit if node == nullptr
		if(nodes_.empty() || node == nullptr) {
			return false;
		}

		// Remove all connectors joining this node to others
		for(auto & it = connectors_.begin(); it < connectors_.end(); it++) {
			ShaderNode * n;
			std::string _;
			it->GetInput(&n, _);
			// If the node n is the node to remove then remove the connector
			if(n == node) {
				connectors_.erase(it--);
			}
		}

		// TODO - Test whether this actually works
		// NOTE - Remove moves removed elements to end and returns the new end as an iterator
		// NOTE - Erase then deletes element between first arg and last arg from the vector
		size_t size_before { nodes_.size() };
		nodes_.erase(std::remove_if(nodes_.begin(), nodes_.end(), [this, node] (auto & n) {
			return n.get() == node;
		}), nodes_.end());
		return (size_before != nodes_.size());
	}

	// Connect the 2 nodes passed together
	// Returns true iff the nodes were connected
	bool ShaderGraph::Connect(ose::unowned_ptr<ShaderNode> out_node, std::string const & out_node_output, ose::unowned_ptr<ShaderNode> in_node, std::string const & in_node_input)
	{
		// If either node is nullptr, return early
		if(out_node == nullptr || in_node == nullptr) {
			return false;
		}

		// Check if the connector already exists
		for(auto & c : connectors_) {
			ShaderNode * c_out_node, * c_in_node;
			std::string c_out_node_output, c_in_node_input;
			c.GetOutput(&c_out_node, c_out_node_output);
			c.GetInput(&c_in_node, c_in_node_input);
			// Return early if the proposed connector is identical to another
			if(c_out_node == out_node && c_in_node == in_node && out_node_output == c_out_node_output && in_node_input == c_in_node_input) {
				return false;
			}
		}

		// TODO - Test that c_out_node_output and c_in_node_input exist in the ShaderNode type

		// Add the connector to the list of connectors
		return connectors_.emplace_back( in_node, in_node_input, out_node, out_node_output ), true;
	}

	// Get a list of all nodes which connect to into node n
	std::vector<ose::unowned_ptr<NodeConnector>> const ShaderGraph::GetIncomingConnectors(ose::unowned_ptr<ShaderNode> n)
	{
		std::vector<ose::unowned_ptr<NodeConnector>> incoming_connectors;
		for(auto & c : connectors_)
		{
			ose::unowned_ptr<ShaderNode> out_node;
			std::string out_name;
			c.GetOutput(&out_node, out_name);
			// If the output of node is n, add c to the list
			if(out_node == n)
			{
				incoming_connectors.push_back(&c);
			}
		}
		return incoming_connectors;
	}
}
