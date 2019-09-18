#pragma once
#include "ShaderNode.h"
#include "NodeConnector.h"

namespace ose::shader
{
	class Shader
	{
	public:
		Shader(const std::string & name);
		~Shader();

		// ShaderNodeType is the type of ShaderNode to add
		// Returns a pointer to the ShaderNode
		// Do not delete or move the pointer returned
		// To delete the node, use the RemoveNode method
		template <typename ShaderNodeType, typename ...Args>
		ShaderNode * AddNode(Args &&... params)
		{
			nodes_.emplace_back( std::make_unique<ShaderNodeType>(std::forward<Args>(params)...) );
			return nodes_.back().get();
		}

		// Remove a shader node from the shader
		// Returns true iff the node exists and is removed successfully
		// TODO - Remove all connectors joining this node to others
		bool RemoveNode(ShaderNode * node);

		// Connect the 2 nodes passed together
		// Returns true iff the nodes were connected
		bool Connect(ShaderNode * out_node, std::string const & out_node_output, ShaderNode * in_node, std::string const & in_node_input);

		// Returns a const list of all nodes in the shader graph
		const std::vector<std::unique_ptr<ShaderNode>> & GetNodes() const { return nodes_; }

	private:
		// Every shader has a name
		std::string name_;

		// List of all nodes in the graph
		std::vector<std::unique_ptr<ShaderNode>> nodes_;

		// List of connectors between nodes
		std::vector<NodeConnector> connectors_;
	};
}

