#include "pch.h"
#include "ShaderProgGLSL.h"

namespace ose::shader
{
	ShaderProgGLSL::ShaderProgGLSL() : ShaderProg()
	{

	}

	ShaderProgGLSL::~ShaderProgGLSL()
	{

	}

	// Build a shader object from a shader graph
	void ShaderProgGLSL::BuildShaderGraph(ShaderGraph & shader_graph)
	{
		// TODO - Remove
		// TEST - Builds default 2d shader
		GLuint vert = glCreateShader(GL_VERTEX_SHADER);
		// TODO - Possibly replace sampler scaling by scaling the instance transform by the texture size
		char const * vert_source =
			"#version 330\n"
			"layout(location = 0) in vec2 position;\n"
			"layout(location = 1) in vec2 uv;\n"
			"out vec2 vertexUV;\n"
			"uniform mat4 viewProjMatrix;\n"
			"uniform mat4 worldTransform;\n"
			"uniform sampler2D texSampler;\n"
			"void main() {\n"
			"	vertexUV = uv;\n"
			"	vec4 samplerSize = vec4(textureSize(texSampler, 0), 1, 1);"
			"	gl_Position = samplerSize * viewProjMatrix * worldTransform * vec4(position, 0.0, 1.0);\n"
			"}\n"
		;
		glShaderSource(vert, 1, &vert_source, NULL);
		glCompileShader(vert);

		GLint isCompiled = 0;
		glGetShaderiv(vert, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vert, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(vert, maxLength, &maxLength, &errorLog[0]);
			std::string msg(errorLog.begin(), errorLog.end());
			LOG_ERROR(msg);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(vert); // Don't leak the shader.
			return;
		}

		GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
		char const * frag_source =
			"#version 330\n"
			"layout(location = 0) out vec4 fragColor;\n"
			"in vec2 vertexUV;\n"
			"uniform sampler2D texSampler;\n"
			"void main() {\n"
			"	fragColor = texture(texSampler, vertexUV);\n"
			"}\n"
		;
		glShaderSource(frag, 1, &frag_source, NULL);
		glCompileShader(frag);

		isCompiled = 0;
		glGetShaderiv(frag, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(frag, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(frag, maxLength, &maxLength, &errorLog[0]);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(frag); // Don't leak the shader.
			return;
		}

		shader_prog_ = glCreateProgram();
		glAttachShader(shader_prog_, vert);
		glAttachShader(shader_prog_, frag);
		glLinkProgram(shader_prog_);

		GLint isLinked = 0;
		glGetProgramiv(shader_prog_, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(shader_prog_, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(shader_prog_, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(shader_prog_);
			// Don't leak shaders either.
			glDeleteShader(vert);
			glDeleteShader(frag);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			return;
		}

		glDetachShader(shader_prog_, vert);
		glDetachShader(shader_prog_, frag);

		glUseProgram(shader_prog_);
		glUniform1i(glGetUniformLocation(shader_prog_, "texSampler"), 0);

		// TODO - Work backwards from output color node
		// Start code in the lowest level shader (fragment)
		// Whenever a node is encountered requiring the higher level, ...
		// ...begin that shader & bridge connection between the two shaders
	///	ProcessIncomingConnectors(shader_graph, shader_graph.GetOutputColorNode());
	}

	// Process the incoming connectors of node n
	void ShaderProgGLSL::ProcessIncomingConnectors(ShaderGraph & shader_graph, unowned_ptr<ShaderNode> n)
	{
		shader_graph.GetIncomingConnectors(shader_graph.GetOutputColorNode());
		// TODO - Determine type of shader node and then generate code for it
	}
}
