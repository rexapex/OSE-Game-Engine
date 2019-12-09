#include "pch.h"
#include "RenderPoolGL.h"
#include "ERenderObjectType.h"

//#include "OSE-Core/EngineReferences.h"

#include "OSE-Core/Entity/SpriteRenderer.h"

// TODO - Remove
#include "OSE-Core/Shader/Shaders/ShaderGraphDefault.h"
#include "ShaderGroupGL.h"

namespace ose::rendering
{
	using namespace math;
	using namespace shader;
	using namespace entity;

	RenderPoolGL::RenderPoolGL()
	{
		
	}

	RenderPoolGL::~RenderPoolGL()
	{

	}

	// Initialise the render pool
	void RenderPoolGL::Init()
	{
		GLuint prog;

		// TODO - Remove
		{
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
				"	mat4 samplerScale;"
				"	samplerScale[0][0] = samplerSize[0];"
				"	samplerScale[1][1] = samplerSize[1];"
				"	samplerScale[2][2] = samplerSize[2];"
				"	samplerScale[3][3] = 1.0;"
				"	gl_Position = (samplerScale * (viewProjMatrix * worldTransform)) * vec4(position, 0.0, 1.0);\n"
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
				ERROR_LOG(msg);

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

			prog = glCreateProgram();
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

			glUseProgram(prog);
			glUniform1i(glGetUniformLocation(prog, "texSampler"), 0);
		}

		render_passes_.emplace_back();
		ShaderGroupGL sg;
		sg.shader_prog_ = prog;
		render_passes_[0].shader_groups_.push_back(sg);
	}

	// Add a sprite renderer component to the render pool
	void RenderPoolGL::AddSpriteRenderer(ITransform const & t, ose::unowned_ptr<SpriteRenderer> sr)
	{
		// Try to find a render object of the same type
		ShaderGroupGL & s = render_passes_[0].shader_groups_[0];
		bool found_sprite_renderer_group { false };
		for(auto & r : s.render_objects_)
		{
			if(r.type_ == ERenderObjectType::SPRITE_RENDERER)
			{
				// Add the sprite renderer to the existing render object
				found_sprite_renderer_group = true;
				r.textures_.push_back(static_cast<ose::unowned_ptr<TextureGL const>>(sr->GetTexture()));
				s.render_objects_.back().transforms_.push_back(t.GetTransformMatrix());
			}
		}
		// If the sprite renderer group could not be found, make one
		if(!found_sprite_renderer_group)
		{
			// Create a VBO for the render object
			GLuint vbo;
			glGenBuffers(1, &vbo);
			// Data consists of 2-float position and 2-float tex coords interleaved
			float data[] = {
				0, 0, 0, 1,
				0, 1, 0, 0,
				1, 1, 1, 0,
				1, 0, 1, 1
			};
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

			// Create a VAO for the render object
			GLuint vao;
			glGenBuffers(1, &vao);
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			// TODO - Vertex attrib locations are to be controlled by the built shader program
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)(2 * sizeof(float)));

			s.render_objects_.emplace_back();
			s.render_objects_.back().vbo_ = vbo;
			s.render_objects_.back().vao_ = vao;
			s.render_objects_.back().type_ = ERenderObjectType::SPRITE_RENDERER;
			s.render_objects_.back().textures_.push_back(static_cast<ose::unowned_ptr<TextureGL const>>(sr->GetTexture()));
			s.render_objects_.back().transforms_.push_back(t.GetTransformMatrix());
		}
	}
}
