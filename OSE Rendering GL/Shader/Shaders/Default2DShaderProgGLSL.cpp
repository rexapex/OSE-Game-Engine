#include "pch.h"
#include "Default2DShaderProgGLSL.h"

namespace ose::shader
{
	Default2DShaderProgGLSL::Default2DShaderProgGLSL() : ShaderProg(nullptr), shader_prog_(0)
	{
		CreateShaderProg();
	}

	Default2DShaderProgGLSL::~Default2DShaderProgGLSL()
	{
		DestroyShaderProg();
	}

	// Build an OpenGL shader object from a shader graph
	void Default2DShaderProgGLSL::CreateShaderProg()
	{
		if(shader_prog_)
			return;

		// TEST - Builds default 2d shader
		GLuint vert = glCreateShader(GL_VERTEX_SHADER);
		// TODO - Possibly replace sampler scaling by scaling the instance transform by the texture size
		char const * vert_source =
			"#version 330\n"
			"layout(location = 0) in vec2 position;\n"
			"layout(location = 1) in vec2 uv;\n"
			"out vec2 vertexUV;\n"
			//"out vec3 vertexCamSpacePos;\n"
			"uniform mat4 viewProjMatrix;\n"
			"uniform mat4 worldTransform;\n"
			"uniform sampler2D texSampler;\n"
			"void main() {\n"
			"	vertexUV = uv;\n"
			//"	vertexCamSpacePos = vec3(position, 0);\n"
			"	vec4 samplerSize = vec4(textureSize(texSampler, 0), 1, 1);"
			"	mat4 samplerScale;"
			"	samplerScale[0][0] = samplerSize[0];"
			"	samplerScale[1][1] = samplerSize[1];"
			"	samplerScale[2][2] = samplerSize[2];"
			"	samplerScale[3][3] = 1.0;"
			"	gl_Position = (viewProjMatrix * (worldTransform * samplerScale)) * vec4(position, 0.0, 1.0);\n"
			//	"	gl_Position = ((viewProjMatrix * worldTransform)) * vec4(position, 0.0, 1.0);\n"
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
			//"layout(location = 0) out vec3 gPos;\n"
			//"layout(location = 1) out vec3 gNormal;\n"
			//"layout(location = 2) out vec4 gColourSpec;\n"
			"in vec2 vertexUV;\n"
			//"in vec3 vertexCamSpacePos;\n"
			"uniform sampler2D texSampler;\n"
			"void main() {\n"
			//"	gPos = vertexCamSpacePos;\n"
			//"	gNormal = vec3(0, 0, 1)\n;"
			//"	gColourSpec = texture(texSampler, vertexUV);\n"
			"	fragColor = texture(texSampler, vertexUV);\n"
			//"	float gamma = 2.2;\n"
			//"	fragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));\n"
			//"	fragColor = vec4(1, 0, 0, 1);\n"
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
		glUniform1i(glGetUniformLocation(prog, "texSampler"), 0);

		shader_prog_ = prog;
	}

	// Destroy the OpenGL shader object
	void Default2DShaderProgGLSL::DestroyShaderProg()
	{
		if(shader_prog_)
			glDeleteProgram(shader_prog_);
	}
}
