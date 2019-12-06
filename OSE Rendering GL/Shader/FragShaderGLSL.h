#pragma once

namespace ose::shader
{
	class FragShaderGLSL
	{
	public:
		FragShaderGLSL();
		~FragShaderGLSL();

		// Build the contents of the frag shader into GLSL source code
		std::string Build();

		// Add a function to the frag shader
		// Decleration should exclude {} or ;
		// Definition should exclude {}
		void AddFunction(std::string const & decleration, std::string const & body);

	private:
		// Contents of the main function
		std::string main_;

		// Contents of the header (functions declerations and other glsl related code)
		std::string header_;

		// Contents of the body (function definitions)
		std::string body_;
	};
}

