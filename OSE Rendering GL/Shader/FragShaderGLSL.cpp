#include "pch.h"
#include "FragShaderGLSL.h"

namespace ose::shader
{
	FragShaderGLSL::FragShaderGLSL()
	{

	}

	FragShaderGLSL::~FragShaderGLSL()
	{

	}

	// Build the contents of the frag shader into GLSL source code
	std::string FragShaderGLSL::Build()
	{
		return
			"#version 330\n" +
			header_ +
			"void main() {\n" +
			main_ +
			"}\n" +
			body_;
	}

	// Add a function to the frag shader
	// Decleration should exclude {} or ;
	// Definition should exclude {}
	void FragShaderGLSL::AddFunction(std::string const & decleration, std::string const & body)
	{
		header_ += "\n" + decleration + ";\n";
		body_ += "\n" + decleration + "\n{\n" + body + "}\n";
	}
}
