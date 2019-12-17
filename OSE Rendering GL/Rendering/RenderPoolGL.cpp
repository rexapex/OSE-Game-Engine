#include "pch.h"
#include "RenderPoolGL.h"
#include "ERenderObjectType.h"

#include "OSE-Core/Entity/Component/SpriteRenderer.h"
#include "OSE-Core/Entity/Component/TileRenderer.h"

namespace ose::rendering
{
	using namespace math;
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
				//"	gl_Position = (samplerScale * (viewProjMatrix * worldTransform)) * vec4(position, 0.0, 1.0);\n"
				"	gl_Position = ((viewProjMatrix * worldTransform)) * vec4(position, 0.0, 1.0);\n"
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

			glUseProgram(prog);
			glUniform1i(glGetUniformLocation(prog, "texSampler"), 0);

			render_passes_.emplace_back();
			ShaderGroupGL sg;
			sg.shader_prog_ = prog;
			render_passes_[0].shader_groups_.push_back(sg);
		}
	}

	// Add a sprite renderer component to the render pool
	void RenderPoolGL::AddSpriteRenderer(ITransform const & t, ose::unowned_ptr<SpriteRenderer> sr)
	{
		if(sr->GetTexture() == nullptr)
			return;

		// Try to find a render object of the same type
		ShaderGroupGL & s = render_passes_[0].shader_groups_[0];
		bool found_group { false };
		for(auto & r : s.render_objects_)
		{
			if(r.type_ == ERenderObjectType::SPRITE_RENDERER)
			{
				// Add the sprite renderer to the existing render object
				found_group = true;
				r.textures_.push_back(static_cast<ose::unowned_ptr<TextureGL const>>(sr->GetTexture()));
				s.render_objects_.back().transforms_.push_back(t.GetTransformMatrix());
			}
		}
		// If the sprite renderer group could not be found, make one
		if(!found_group)
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
			s.render_objects_.back().first_ = 0;
			s.render_objects_.back().count_ = 4;
			s.render_objects_.back().render_primitive_ = GL_QUADS;
			s.render_objects_.back().type_ = ERenderObjectType::SPRITE_RENDERER;
			s.render_objects_.back().textures_.push_back(static_cast<ose::unowned_ptr<TextureGL const>>(sr->GetTexture()));
			s.render_objects_.back().transforms_.push_back(t.GetTransformMatrix());
		}
	}

	// Add a tile renderer component to the render pool
	void RenderPoolGL::AddTileRenderer(ose::math::ITransform const & t, ose::unowned_ptr<ose::entity::TileRenderer> tr)
	{
		if(tr->GetTexture() == nullptr)
			return;

		// Unlike SpriteRenderer, TileRenderer cannot share a group since the tile grid is encoded into the buffer data
		ShaderGroupGL & s = render_passes_[0].shader_groups_[0];

		uint32_t tile_width  { static_cast<uint32_t>(tr->GetTexture()->GetWidth()) / tr->GetNumCols() };
		uint32_t tile_height { static_cast<uint32_t>(tr->GetTexture()->GetHeight()) / tr->GetNumRows() };

		// TODO
		size_t tilemap_width  { 10 };
		size_t tilemap_height { 10 };

		// TODO
		uint32_t spacing_x { tile_width };
		uint32_t spacing_y { tile_height };
		// TODO - Was incorrectly adding to x/y instead of u/v
		float half_pixel_width  = 0;// { (1.0f / tr->GetTexture()->GetWidth()) / 2};
		float half_pixel_height = 0;//{ (1.0f / tr->GetTexture()->GetHeight()) / 2};

		// Create a VBO for the render object
		GLuint vbo;
		glGenBuffers(1, &vbo);
		// Data consists of 2-float position and 2-float tex coords interleaved, each tile is composed of 2 tris (6 vertices)
		std::vector<float> data(6 * 4 * tilemap_width * tilemap_height);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		for(int j = 0; j < tilemap_height; j++)
		{
			for(int i = 0; i < tilemap_width; i++)
			{
				// TODO
				int value { i % 4 };
				// Calculate the position of the tile in the texture atlas
				int atlas_x { value % static_cast<int>(tr->GetNumCols()) };
				int atlas_y { value % static_cast<int>(tr->GetNumRows()) };
				// Calculate the position co-ordinates for the tile
				float x0 = i * spacing_x + half_pixel_width;
				float x1 = i * spacing_x + tile_width - half_pixel_width;
				float y0 = j * spacing_y + half_pixel_height;
				float y1 = j * spacing_y + tile_height - half_pixel_height;
				// Calculate the texture co-ordinates for the tile
				float u0 = (float)atlas_x / tr->GetNumCols();
				float u1 = (float)(atlas_x + 1) / tr->GetNumCols();
				float v0 = (float)atlas_y / tr->GetNumRows();
				float v1 = (float)(atlas_y + 1) / tr->GetNumRows();
				// Set the vertex's position and texture co-ordinates
				size_t tile_offset { 6*4*(i + tilemap_width * (tilemap_height - j - 1)) };
				// Top Left
				data[tile_offset + 0*4 + 0] = x0;
				data[tile_offset + 0*4 + 1] = y1;
				data[tile_offset + 0*4 + 2] = u0;
				data[tile_offset + 0*4 + 3] = v0;
				// Top Right
				data[tile_offset + 1*4 + 0] = x1;
				data[tile_offset + 1*4 + 1] = y1;
				data[tile_offset + 1*4 + 2] = u1;
				data[tile_offset + 1*4 + 3] = v0;
				// Bottom Right
				data[tile_offset + 2*4 + 0] = x1;
				data[tile_offset + 2*4 + 1] = y0;
				data[tile_offset + 2*4 + 2] = u1;
				data[tile_offset + 2*4 + 3] = v1;
				// Bottom Right
				data[tile_offset + 3*4 + 0] = x1;
				data[tile_offset + 3*4 + 1] = y0;
				data[tile_offset + 3*4 + 2] = u1;
				data[tile_offset + 3*4 + 3] = v1;
				// Bottom Left
				data[tile_offset + 4*4 + 0] = x0;
				data[tile_offset + 4*4 + 1] = y0;
				data[tile_offset + 4*4 + 2] = u0;
				data[tile_offset + 4*4 + 3] = v1;
				// Top Left
				data[tile_offset + 5*4 + 0] = x0;
				data[tile_offset + 5*4 + 1] = y1;
				data[tile_offset + 5*4 + 2] = u0;
				data[tile_offset + 5*4 + 3] = v0;
			}
		}
		glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(float), data.data(), GL_STATIC_DRAW);

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
		s.render_objects_.back().first_ = 0;
		s.render_objects_.back().count_ = 6 * tilemap_width * tilemap_height;
		s.render_objects_.back().render_primitive_ = GL_TRIANGLES;
		s.render_objects_.back().type_ = ERenderObjectType::TILE_RENDERER;
		s.render_objects_.back().textures_.push_back(static_cast<ose::unowned_ptr<TextureGL const>>(tr->GetTexture()));
		s.render_objects_.back().transforms_.push_back(t.GetTransformMatrix());
	}
}
