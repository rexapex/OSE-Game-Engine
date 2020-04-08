#pragma once
#include "OSE-Core/Shader/ShaderProg.h"
#include "OSE-Core/Shader/ShaderNode.h"

namespace ose::shader
{
	struct ShaderLayer;

	class ShaderProgGLSL final : public ShaderProg
	{
	public:
		ShaderProgGLSL(std::unique_ptr<ShaderGraph> shader_graph);
		virtual ~ShaderProgGLSL();

		// Build an OpenGL shader object from a shader graph
		void CreateShaderProg() override;

		// Destroy the OpenGL shader object
		void DestroyShaderProg() override;

		// Get the shader program id
		uint32_t GetShaderProgId() const { return shader_prog_; }

	private:
		// OpenGL shader program id
		uint32_t shader_prog_ { 0 };

		// Split the shader graph nodes into layers
		// All nodes in a layer can be computed simultaneously
		void CreateLayers(std::vector<ShaderLayer> & layers, std::vector<unowned_ptr<ShaderNode>> & expended_nodes);

		// Determine which shader type is required for each layer
		// If nodes within the same shader require different shader types, split the layer into multiple layers
		void DetermineLayerTypes(std::vector<ShaderLayer> & layers);

		// Generate GLSL source code from a list of shader layers
		// Outputs source code into _src arguments
		void GenerateSourceCode(std::vector<ShaderLayer> & layers, std::string & vert_src, std::string & frag_src);

		// Create a GLSL shader object from source code
		//uint32_t CreateShaderObject();
	};
}

