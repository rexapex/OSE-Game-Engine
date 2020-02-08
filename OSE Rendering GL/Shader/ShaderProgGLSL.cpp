#include "pch.h"
#include "ShaderProgGLSL.h"

namespace ose::shader
{
	ShaderProgGLSL::ShaderProgGLSL(std::unique_ptr<ShaderGraph> shader_graph) : ShaderProg(std::move(shader_graph))
	{

	}

	ShaderProgGLSL::~ShaderProgGLSL()
	{

	}

	// Build a shader object from a shader graph
	void ShaderProgGLSL::CreateShaderProg()
	{
		if(shader_graph_ == nullptr)
			return;

		// TODO - Work backwards from output color node
		// Start code in the lowest level shader (fragment)
		// Whenever a node is encountered requiring the higher level, ...
		// ...begin that shader & bridge connection between the two shaders
	///	ProcessIncomingConnectors(shader_graph, shader_graph.GetOutputColorNode());
		
		// Split the nodes into layers
		// Nodes in a layer can be computed simultaneously
		std::vector<std::vector<unowned_ptr<ShaderNode>>> layers;
		std::vector<unowned_ptr<ShaderNode>> expended_nodes;
		CreateLayers(layers, expended_nodes);
	}

	// Destroy the OpenGL shader object
	void ShaderProgGLSL::DestroyShaderProg()
	{
		// TODO
	}

	// Split the shader graph nodes into layers
	// All nodes in a layer can be computed simultaneously
	void ShaderProgGLSL::CreateLayers(std::vector<std::vector<unowned_ptr<ShaderNode>>> & layers, std::vector<unowned_ptr<ShaderNode>> & expended_nodes)
	{
		layers.emplace_back();
		for(auto & node : shader_graph_->GetNodes())
		{
			bool can_add { std::find(expended_nodes.begin(), expended_nodes.end(), node.get()) == expended_nodes.end() };
			for(auto & incoming_connector : shader_graph_->GetIncomingConnectors(node.get()))
			{
				unowned_ptr<ShaderNode> out_node;
				std::string out_name;
				incoming_connector->GetOutput(&out_node, out_name);
				if(std::find(expended_nodes.begin(), expended_nodes.end(), out_node) == expended_nodes.end())
					can_add = false;
			}
			if(can_add)
			{
				layers.back().push_back(node.get());
				expended_nodes.push_back(node.get());
			}
		}
		// If there are nodes left, create a new layer
		if(layers.back().size() > 0)
			CreateLayers(layers, expended_nodes);
	};

	// Process the incoming connectors of node n
	void ShaderProgGLSL::ProcessIncomingConnectors(ShaderGraph & shader_graph, unowned_ptr<ShaderNode> n)
	{
		for(auto node : shader_graph.GetIncomingConnectors(shader_graph.GetOutputColorNode()))
		{
			
		}
	}
}
