#include "pch.h"
#include "ShaderProgGLSL.h"
#include "ShaderLayer.h"
#include "OSE-Core/Shader/EShaderType.h"

#include "OSE-Core/Shader/Nodes/BRDFNode.h"
#include "OSE-Core/Shader/Nodes/OutputNode.h"
#include "OSE-Core/Shader/Nodes/RGBANode.h"
#include "OSE-Core/Shader/Nodes/TextureNode.h"
#include "OSE-Core/Shader/Nodes/VectorNode.h"

namespace ose::shader
{
	ShaderProgGLSL::ShaderProgGLSL(std::unique_ptr<ShaderGraph> shader_graph) : ShaderProg(std::move(shader_graph))
	{

	}

	ShaderProgGLSL::~ShaderProgGLSL()
	{
		DestroyShaderProg();
	}

	// Build a shader object from a shader graph
	void ShaderProgGLSL::CreateShaderProg()
	{
		if(shader_graph_ == nullptr)
			return;

		// Split the nodes into layers
		// Nodes in a layer can be computed simultaneously
		std::vector<ShaderLayer> layers;
		std::vector<ShaderNode *> expended_nodes;
		CreateLayers(layers, expended_nodes);

		// Determine which shader type is required for each layer
		// If nodes within the same shader require different shader types, split the layer into multiple layers
		DetermineLayerTypes(layers);

		// Generate source code for the layers
		std::string vert_src, frag_src;
		GenerateSourceCode(layers, vert_src, frag_src);
	}

	// Destroy the OpenGL shader object
	void ShaderProgGLSL::DestroyShaderProg()
	{
		if(shader_prog_)
			glDeleteShader(shader_prog_);
	}

	// Split the shader graph nodes into layers
	// All nodes in a layer can be computed simultaneously
	void ShaderProgGLSL::CreateLayers(std::vector<ShaderLayer> & layers, std::vector<ShaderNode *> & expended_nodes)
	{
		layers.emplace_back();
		for(auto & node : shader_graph_->GetNodes())
		{
			bool can_add { std::find(expended_nodes.begin(), expended_nodes.end(), node.get()) == expended_nodes.end() };
			for(auto & incoming_connector : shader_graph_->GetIncomingConnectors(node.get()))
			{
				ShaderNode * out_node;
				std::string out_name;
				incoming_connector->GetOutput(&out_node, out_name);
				if(std::find(expended_nodes.begin(), expended_nodes.end(), out_node) == expended_nodes.end())
					can_add = false;
			}
			if(can_add)
			{
				layers.back().nodes_.push_back(node.get());
				expended_nodes.push_back(node.get());
			}
		}
		// If there are nodes left, create a new layer
		if(layers.back().nodes_.size() > 0)
			CreateLayers(layers, expended_nodes);
	};

	// Determine which shader type is required for each layer
	// If nodes within the same shader require different shader types, split the layer into multiple layers
	void ShaderProgGLSL::DetermineLayerTypes(std::vector<ShaderLayer> & layers)
	{
		// Do as much as possible in the latest shader for better looking output
		std::vector<int> shader_type_levels { ST_VERTEX, ST_FRAGMENT };
		int level = shader_type_levels.size()-1;

		// Loop over layers in reverse
		for(auto iter = layers.rbegin(); iter != layers.rend(); ++iter)
		{
			// If the level is less than zero, the shader is impossible to compile
			if(level < 0)
			{
				LOG_ERROR("Failed to validate shader graph");
				return;
			}

			if(iter->nodes_.size() == 0)
				continue;

			std::vector<ShaderNode *> current_level_nodes;
			std::vector<ShaderNode *> lower_level_nodes;
			for(auto node : iter->nodes_)
			{
				if(node->IsShaderType(shader_type_levels[level]))
				{
					current_level_nodes.push_back(node);
				}
				else
				{
					lower_level_nodes.push_back(node);
				}
			}

			// If all the nodes belong on a lower level, set the layer level to a lower level then try this layer again
			if(current_level_nodes.size() == 0)
			{
				--level;
				--iter;
				continue;
			}

			// Some of the nodes belong on the current level, therefore, set the layer level
			iter->type_ = shader_type_levels[level];

			// If some (but not all) of the nodes belong on a lower level, split the layer into parts
			if(lower_level_nodes.size() > 0)
			{
				--level;
				ShaderLayer new_layer;
				new_layer.nodes_ = lower_level_nodes;
				layers.insert(iter.base(), new_layer);
				--iter;
				continue;
			}
		}
	}

	// Generate GLSL source code from a list of shader layers
	// Outputs source code into _src arguments
	void ShaderProgGLSL::GenerateSourceCode(std::vector<ShaderLayer> & layers, std::string & vert_src, std::string & frag_src)
	{
		std::string vert_inputs = "";
		std::string vert_outputs = "";
		std::string vert_uniforms = "";
		std::string vert_functions = "";
		std::string vert_main_function = "";

		std::string frag_inputs = "";
		std::string frag_outputs = "";
		std::string frag_uniforms = "";
		std::string frag_functions = "";
		std::string frag_main_function = "";

		auto process_node = [](ShaderNode * node, std::string & inputs, std::string & outputs,
			std::string & uniforms, std::string & functions, std::string & main_function)
		{
			if(node->IsClassType(BRDFNode::GetClassType()))
			{

			}
			else if(node->IsClassType(OutputNode::GetClassType()))
			{

			}
			else if(node->IsClassType(RGBANode::GetClassType()))
			{

			}
			else if(node->IsClassType(TextureNode::GetClassType()))
			{

			}
			else if(node->IsClassType(TextureNode::GetClassType()))
			{

			}
		};

		for(auto & layer : layers)
		{
			if(layer.type_ != ST_UNKNOWN)
			{
				for(auto node : layer.nodes_)
				{
					process_node(node,
						layer.type_ == ST_VERTEX ? vert_inputs : frag_inputs,
						layer.type_ == ST_VERTEX ? vert_outputs : frag_outputs,
						layer.type_ == ST_VERTEX ? vert_uniforms : frag_uniforms,
						layer.type_ == ST_VERTEX ? vert_functions : frag_functions,
						layer.type_ == ST_VERTEX ? vert_main_function : frag_main_function);
				}
			}
		}
	}
}
