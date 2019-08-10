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
		// returns the index of the ShaderNode in to the get_nodes() array
		template <typename ShaderNodeType, typename ...Args>
		int32_t AddNode(Args &&... params)
		{
			nodes_.emplace_back( std::make_unique<ShaderNodeType>(std::forward<Args>(params)...) );
			return nodes_.size() - 1;	// return the index of the new node
		}

		// connect the 2 nodes given at the indexes passed
		// returns true iff the nodes were connected
		bool Connect(int32_t node_index_1, int32_t node_index_2);

		// returns a const list of all nodes in the shader graph
		const std::vector<std::unique_ptr<ShaderNode>> & GetNodes() const { return nodes_; }

	private:
		// every shader has a name
		std::string name_;

		// list of all nodes in the graph
		std::vector<std::unique_ptr<ShaderNode>> nodes_;

		// list of connectors between nodes
		std::vector<NodeConnector> connectors_;
	};
}

