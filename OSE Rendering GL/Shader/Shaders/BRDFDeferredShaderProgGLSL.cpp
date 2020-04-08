#include "pch.h"
#include "BRDFDeferredShaderProgGLSL.h"

namespace ose::shader
{
	BRDFDeferredShaderProgGLSL::BRDFDeferredShaderProgGLSL() : ShaderProg(nullptr), shader_prog_(0)
	{
		CreateShaderProg();
	}

	BRDFDeferredShaderProgGLSL::~BRDFDeferredShaderProgGLSL()
	{
		DestroyShaderProg();
	}

	// Build an OpenGL shader object from a shader graph
	void BRDFDeferredShaderProgGLSL::CreateShaderProg()
	{
		if(shader_prog_)
			return;

		// Builds the deferred rendering shader
		GLuint vert = glCreateShader(GL_VERTEX_SHADER);
		char const * vert_source =
			"#version 330\n"
			"layout (location = 0) in vec2 position;\n"
			"layout (location = 1) in vec2 texCoords;\n"
			"out vec2 TexCoords;\n"
			"void main() {\n"
			"	gl_Position = vec4(position.x, position.y, 0.0, 1.0);\n"
			"	TexCoords = texCoords;\n"
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
			"in vec2 TexCoords;\n"
			"out vec4 outColor;\n"
			"uniform sampler2D gPos;\n"
			"uniform sampler2D gNormal;\n"
			"uniform sampler2D gColourSpec;\n"
			"struct PointLight {\n"
			"	vec4 position;\n"
			"	vec4 color;\n"
			"	float intensity;\n"
			"};\n"
			"uniform PointLight pointLights[16];\n"
			"uniform int numPointLights;\n"
			"void main() {\n"
			"	vec3 fragPos	   = texture(gPos, TexCoords).rgb;\n"
			"	vec3 surfaceNormal = texture(gNormal, TexCoords).rgb;\n"
			"	vec4 texColor	   = vec4(texture(gColourSpec, TexCoords).rgb, 1.0);\n"
			"	float shininess	   = texture(gColourSpec, TexCoords).a;\n"
			"	vec4 color = vec4(0, 0, 0, 1);\n"
			"	for(int i = 0; i < 16 && i < numPointLights; ++i) {\n"
			"		color += pointLights[i].color;\n"
			"	}\n"
			"	color *= texColor;\n"
			//"	float gamma = 2.2;\n"
			//"	colour.rgb = pow(colour.rgb, vec3(1.0/gamma));\n"
			//"	colour = vec4(1, 1, 0, 1);\n"
			"	outColor = color;\n"
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

		GLuint prog = glCreateProgram();
		glAttachShader(prog, vert);
		glAttachShader(prog, frag);
		glLinkProgram(prog);

		GLint isLinked = 0;
		glGetProgramiv(prog, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(prog, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(prog);
			// Don't leak shaders either.
			glDeleteShader(vert);
			glDeleteShader(frag);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			return;
		}

		glDetachShader(prog, vert);
		glDetachShader(prog, frag);
		glDeleteShader(vert);
		glDeleteShader(frag);

		glUseProgram(prog);
		glUniform1i(glGetUniformLocation(prog, "gPos"), 0);
		glUniform1i(glGetUniformLocation(prog, "gNormal"), 1);
		glUniform1i(glGetUniformLocation(prog, "gColourSpec"), 2);

		shader_prog_ = prog;
	}

	// Destroy the OpenGL shader object
	void BRDFDeferredShaderProgGLSL::DestroyShaderProg()
	{
		glDeleteProgram(shader_prog_);
	}
}
