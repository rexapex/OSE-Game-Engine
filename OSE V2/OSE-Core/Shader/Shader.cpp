#include "stdafx.h"
#include "Shader.h"

namespace ose::shader
{
	Shader::Shader(const std::string & name) : name_(name)
	{

	}

	Shader::~Shader()
	{

	}

	// Remove a shader node from the shader
	// Returns true iff the node exists and is removed successfully
	bool Shader::RemoveNode(ShaderNode * node)
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
	bool Shader::Connect(ShaderNode * out_node, std::string const & out_node_output, ShaderNode * in_node, std::string const & in_node_input)
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
}
