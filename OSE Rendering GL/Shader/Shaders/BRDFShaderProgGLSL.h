#pragma once
#include "OSE-Core/Shader/ShaderProg.h"

namespace ose::shader
{
	class BRDFShaderProgGLSL final : public ShaderProg
	{
	public:
		BRDFShaderProgGLSL();
		virtual ~BRDFShaderProgGLSL();

		// Build an OpenGL shader object from a shader graph
		void CreateShaderProg() override;

		// Destroy the OpenGL shader object
		void DestroyShaderProg() override;

		// Get the shader program id
		uint32_t GetShaderProgId() const { return shader_prog_; }

	private:
		// OpenGL shader program id
		uint32_t shader_prog_ { 0 };
	};
}