#include "pch.h"
#include "RenderPoolGL.h"
#include "ERenderObjectType.h"

#include "OSE-Core/Entity/Component/SpriteRenderer.h"
#include "OSE-Core/Entity/Component/TileRenderer.h"
#include "OSE-Core/Entity/Component/MeshRenderer.h"

// TODO - Remove
#include "OSE-Core/Math/ITransform.h"

namespace ose::rendering
{
	RenderPoolGL::RenderPoolGL()
	{
		
	}

	RenderPoolGL::~RenderPoolGL()
	{
		for(auto const & render_pass : render_passes_)
		{
			for(auto const & shader_group : render_pass.shader_groups_)
			{
				glDeleteProgram(shader_group.shader_prog_);

				for(auto const & render_object : shader_group.render_objects_)
				{
					glDeleteBuffers(1, &render_object.vbo_);
					glDeleteVertexArrays(1, &render_object.vao_);
				}
			}
		}
	}

	// Initialise the render pool
	void RenderPoolGL::Init(int fbwidth, int fbheight)
	{
		GLuint fbo;

		// Create a deferred shading render pass
		{
			// Create the fbo
			glGenFramebuffers(1, &fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);

			// Create a position buffer attachment
			GLuint pos_buffer;
			glGenTextures(1, &pos_buffer);
			glBindTexture(GL_TEXTURE_2D, pos_buffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, fbwidth, fbheight, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pos_buffer, 0);

			// Create a colour buffer attachment
			GLuint norm_buffer;
			glGenTextures(1, &norm_buffer);
			glBindTexture(GL_TEXTURE_2D, norm_buffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, fbwidth, fbheight, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, norm_buffer, 0);

			// Create a colour + specular buffer attachment
			GLuint col_buffer;
			glGenTextures(1, &col_buffer);
			glBindTexture(GL_TEXTURE_2D, col_buffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, fbwidth, fbheight, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, col_buffer, 0);

			// Tell OpenGL which attachments to use for rendering
			GLenum attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
			glDrawBuffers(3, attachments);

			// Create the rbo for storing rendering depth info
			GLuint rbo_depth;
			glGenRenderbuffers(1, &rbo_depth);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fbwidth, fbheight);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_depth);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			// Check the framebuffer was created successful
			if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
				LOG("Deferred shading framebuffer creation - SUCCESS");
			else
				LOG("Deferred shading framebuffer creation - FAILURE - " << glGetError());
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// Create the VAO and VBO for rendering the fbo
			float vertices[] =
			{
				-1.0f,  1.0f,  0.0f, 1.0f,
				-1.0f, -1.0f,  0.0f, 0.0f,
				1.0f, -1.0f,  1.0f, 0.0f,

				-1.0f,  1.0f,  0.0f, 1.0f,
				1.0f, -1.0f,  1.0f, 0.0f,
				1.0f,  1.0f,  1.0f, 1.0f
			};

			// Create the vao and vbo for rendering the frambuffer
			GLuint vao { 0 };
			GLuint vbo { 0 };
			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)(2 * sizeof(float)));
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

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
				ERROR_LOG(msg);

				// Provide the infolog in whatever manor you deem best.
				// Exit with failure.
				glDeleteShader(vert); // Don't leak the shader.
				return;
			}

			GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
			char const * frag_source =
				"#version 330\n"
				"in vec2 TexCoords;\n"
				"out vec4 outColour;\n"
				"uniform sampler2D gPos;\n"
				"uniform sampler2D gNormal;\n"
				"uniform sampler2D gColourSpec;\n"
				"void main() {\n"
				"	vec3 fragPos	   = texture(gPos, TexCoords).rgb;\n"
				"	vec3 surfaceNormal = texture(gNormal, TexCoords).rgb;\n"
				"	vec4 texColour	   = vec4(texture(gColourSpec, TexCoords).rgb, 1.0);\n"
				"	float shininess	   = texture(gColourSpec, TexCoords).a;\n"
				"	vec4 colour = texColour;\n"
				//"	float gamma = 2.2;\n"
				//"	colour.rgb = pow(colour.rgb, vec3(1.0/gamma));\n"
				//"	colour = vec4(1, 1, 0, 1);\n"
				"	outColour = colour;\n"
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

			render_passes_.emplace_back();
			ShaderGroupGL s;
			s.shader_prog_ = prog;
			render_passes_.back().clear_ = GL_COLOR_BUFFER_BIT;

			GLenum primitive { GL_TRIANGLES };
			GLint first { 0 };
			GLint count { 6 };
			uint32_t object_id { NextComponentId() };
			s.render_objects_.emplace_back(
				std::initializer_list<uint32_t>{ object_id },
				ERenderObjectType::DEFERRED_QUAD,
				vbo, vao,
				primitive, first, count,
				std::initializer_list<GLuint>{
					pos_buffer,
					norm_buffer,
					col_buffer
				}
				//std::initializer_list<glm::mat4>{ t.GetTransformMatrix() }
				//std::initializer_list<ITransform const &>{ t }
			);
			// TODO - Remove
			s.render_objects_.back().transforms_.emplace_back(&deferred_shader_transform_);
			s.render_objects_.back().texture_stride_ = 3;
			render_passes_.back().shader_groups_.push_back(s);
		}

		// Insert a render pass before the deferred render pass to render objects to
		render_passes_.insert(render_passes_.begin(), RenderPassGL{});
		render_passes_[0].fbo_ = fbo;

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
				"out vec3 vertexCamSpacePos;\n"
				"uniform mat4 viewProjMatrix;\n"
				"uniform mat4 worldTransform;\n"
				"uniform sampler2D texSampler;\n"
				"void main() {\n"
				"	vertexUV = uv;\n"
				"	vertexCamSpacePos = vec3(position, 0);\n"
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
				ERROR_LOG(msg);

				// Provide the infolog in whatever manor you deem best.
				// Exit with failure.
				glDeleteShader(vert); // Don't leak the shader.
				return;
			}

			GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
			char const * frag_source =
				"#version 330\n"
				"layout(location = 0) out vec3 gPos;\n"
				"layout(location = 1) out vec3 gNormal;\n"
				"layout(location = 2) out vec4 gColourSpec;\n"
				"in vec2 vertexUV;\n"
				"in vec3 vertexCamSpacePos;\n"
				"uniform sampler2D texSampler;\n"
				"void main() {\n"
				"	gPos = vertexCamSpacePos;\n"
				"	gNormal = vec3(0, 0, 1)\n;"
				"	gColourSpec = texture(texSampler, vertexUV);\n"
				//"	fragColor = texture(texSampler, vertexUV);\n"
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

			ShaderGroupGL sg;
			sg.shader_prog_ = prog;
			render_passes_[0].shader_groups_.push_back(sg);
		}

		// TODO - Remove
		{
			// TEST - Builds default 3d shader
			GLuint vert = glCreateShader(GL_VERTEX_SHADER);
			char const * vert_source =
				"#version 330\n"
				"layout(location = 0) in vec3 position;\n"
				"layout(location = 1) in vec3 normal;\n"
				"layout(location = 2) in vec2 uv;\n"
				"out vec2 vertexUV;\n"
				"out vec3 vertexNormal;\n"
				"out vec3 vertexCamSpacePos;\n"
				"uniform mat4 viewProjMatrix;\n"
				"uniform mat4 worldTransform;\n"
				"uniform sampler2D texSampler;\n"
				"void main() {\n"
				"	vertexUV = uv;\n"
				"	vertexCamSpacePos = position;\n"
				"	vertexNormal = normal;\n"	// TODO - Check OSE V1 for multiplication to apply
				"	gl_Position = (viewProjMatrix * worldTransform) * vec4(position, 1.0);\n"
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
				"layout(location = 0) out vec3 gPos;\n"
				"layout(location = 1) out vec3 gNormal;\n"
				"layout(location = 2) out vec4 gColourSpec;\n"
				"in vec2 vertexUV;\n"
				"in vec3 vertexNormal;\n"
				"in vec3 vertexCamSpacePos;\n"
				"uniform sampler2D texSampler;\n"
				"void main() {\n"
				"	gPos = vertexCamSpacePos;\n"
				"	gNormal = normalize(vertexNormal);\n"
				"	gColourSpec.rgb = texture(texSampler, vertexUV).rgb;\n"
				//"	gColourSpec.a = material.shininess;\n"
				"	gColourSpec.a = 1.0;\n"
				//"	fragColor = texture(texSampler, vertexUV);\n"
				//"	float gamma = 2.2;\n"
				//"	fragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));\n"
				//"	fragColor.a = 1.0;\n"
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

			ShaderGroupGL sg;
			sg.shader_prog_ = prog;
			render_passes_[0].shader_groups_.push_back(sg);
		}
	}

	// Set the size of the framebuffer (required if render pool contains deferred shading render pass)
	void RenderPoolGL::SetFramebufferSize(int width, int height)
	{
		// Update the deferred shading framebuffer object
		// TODO
	}

	// Add a sprite renderer component to the render pool
	void RenderPoolGL::AddSpriteRenderer(ITransform const & t, unowned_ptr<SpriteRenderer> sr)
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
				r.textures_.push_back(static_cast<unowned_ptr<TextureGL const>>(sr->GetTexture())->GetGlTexId());
				//r.transforms_.push_back(t.GetTransformMatrix());
				r.transforms_.push_back(&t);
				uint32_t object_id { NextComponentId() };
				r.component_ids_.push_back(object_id);
				sr->SetEngineData(object_id);
				break;
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
				1, 0, 1, 1,
				1, 1, 1, 0,
				0, 1, 0, 0
			};
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

			// Create a VAO for the render object
			GLuint vao;
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			// TODO - Vertex attrib locations are to be controlled by the built shader program
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)(2 * sizeof(float)));
			// Unbind the vao
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// Add a new render object
			GLenum primitive { GL_QUADS };
			GLint first { 0 };
			GLint count { 4 };
			uint32_t object_id { NextComponentId() };
			s.render_objects_.emplace_back(
				std::initializer_list<uint32_t>{ object_id },
				ERenderObjectType::SPRITE_RENDERER,
				vbo, vao,
				primitive, first, count,
				std::initializer_list<GLuint>{ static_cast<unowned_ptr<TextureGL const>>(sr->GetTexture())->GetGlTexId() }
				//std::initializer_list<glm::mat4>{ t.GetTransformMatrix() }
				//std::initializer_list<ITransform const &>{ t }
			);
			// TODO - Remove
			s.render_objects_.back().transforms_.emplace_back(&t);
			s.render_objects_.back().texture_stride_ = 1;
			sr->SetEngineData(object_id);
		}
	}

	// Add a tile renderer component to the render pool
	void RenderPoolGL::AddTileRenderer(ITransform const & t, unowned_ptr<TileRenderer> tr)
	{
		if(tr->GetTexture() == nullptr || tr->GetTilemap() == nullptr)
			return;

		// Unlike SpriteRenderer, TileRenderer cannot share a group since the tile grid is encoded into the buffer data
		ShaderGroupGL & s = render_passes_[0].shader_groups_[0];

		// Get a reference to the tilemap
		auto & tilemap = *tr->GetTilemap();

		// Calculate tile dimensions s.t. when multiplied by the texture dimensions in the shader, the tiles will be the correct size
		float tile_width  { 1.0f / tr->GetNumCols() };
		float tile_height { 1.0f / tr->GetNumRows() };

		// Get the width and height of the tilemap
		int32_t tilemap_width  { tilemap.GetWidth() };
		int32_t tilemap_height { tilemap.GetHeight() };

		// Get the x and y spacing between tiles
		float spacing_x { tile_width * tr->GetSpacingX() };
		float spacing_y { tile_height * tr->GetSpacingY() };

		// Calculate the dimensions of half a pixel in texture co-ordinate space
		float half_pixel_width  = { (1.0f / tr->GetTexture()->GetWidth()) / 2};
		float half_pixel_height = { (1.0f / tr->GetTexture()->GetHeight()) / 2};

		// Create a VBO for the render object
		GLuint vbo;
		glGenBuffers(1, &vbo);
		// Data consists of 2-float position and 2-float tex coords interleaved, each tile is composed of 2 tris (6 vertices)
		std::vector<float> data(static_cast<size_t>(6) * 4 * tilemap_width * tilemap_height);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		for(size_t j = 0; j < tilemap_height; j++)
		{
			for(size_t i = 0; i < tilemap_width; i++)
			{
				// Get the value of the tile at (x, y) - Stored upside down so use y = j - height - 1 instead of y = j
				int32_t value { tilemap(i, tilemap_height-j-1) };
				if(value >= 0 && value < tr->GetNumTiles())
				{
					// Calculate the position of the tile in the texture atlas
					int32_t atlas_x { value % tr->GetNumCols() };
					int32_t atlas_y { value / tr->GetNumCols() };
					// Calculate the position co-ordinates for the tile
					float x0 = i * spacing_x;
					float x1 = i * spacing_x + tile_width;
					float y0 = j * spacing_y;
					float y1 = j * spacing_y + tile_height;
					// Calculate the texture co-ordinates for the tile
					float u0 = (float)atlas_x / tr->GetNumCols() + half_pixel_width;
					float u1 = (float)(atlas_x + 1) / tr->GetNumCols() - half_pixel_width;
					float v0 = (float)atlas_y / tr->GetNumRows() + half_pixel_height;
					float v1 = (float)(atlas_y + 1) / tr->GetNumRows() - half_pixel_height;
					// Set the vertex's position and texture co-ordinates
					size_t tile_offset { static_cast<size_t>(6*4*(i + (tilemap_height - j - 1)*tilemap_width)) };
					// Top Left
					data[tile_offset + 0*4 + 0] = x0;
					data[tile_offset + 0*4 + 1] = y1;
					data[tile_offset + 0*4 + 2] = u0;
					data[tile_offset + 0*4 + 3] = v0;
					// Bottom Right
					data[tile_offset + 1*4 + 0] = x1;
					data[tile_offset + 1*4 + 1] = y0;
					data[tile_offset + 1*4 + 2] = u1;
					data[tile_offset + 1*4 + 3] = v1;
					// Top Right
					data[tile_offset + 2*4 + 0] = x1;
					data[tile_offset + 2*4 + 1] = y1;
					data[tile_offset + 2*4 + 2] = u1;
					data[tile_offset + 2*4 + 3] = v0;
					// Bottom Right
					data[tile_offset + 3*4 + 0] = x1;
					data[tile_offset + 3*4 + 1] = y0;
					data[tile_offset + 3*4 + 2] = u1;
					data[tile_offset + 3*4 + 3] = v1;
					// Top Left
					data[tile_offset + 4*4 + 0] = x0;
					data[tile_offset + 4*4 + 1] = y1;
					data[tile_offset + 4*4 + 2] = u0;
					data[tile_offset + 4*4 + 3] = v0;
					// Bottom Left
					data[tile_offset + 5*4 + 0] = x0;
					data[tile_offset + 5*4 + 1] = y0;
					data[tile_offset + 5*4 + 2] = u0;
					data[tile_offset + 5*4 + 3] = v1;
				}
			}
		}
		glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(float), data.data(), GL_STATIC_DRAW);

		// Create a VAO for the render object
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		// TODO - Vertex attrib locations are to be controlled by the built shader program
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)(2 * sizeof(float)));
		// Unbind the vao
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Add a new render object
		GLenum primitive { GL_TRIANGLES };
		GLint first { 0 };
		GLint count { 6 * tilemap_width * tilemap_height };
		uint32_t object_id { NextComponentId() };
		s.render_objects_.emplace_back(
			std::initializer_list<uint32_t>{ object_id },
			ERenderObjectType::TILE_RENDERER,
			vbo, vao,
			primitive, first, count,
			std::initializer_list<GLuint>{ static_cast<unowned_ptr<TextureGL const>>(tr->GetTexture())->GetGlTexId() }
			//std::initializer_list<glm::mat4>{ t.GetTransformMatrix() }
			//std::initializer_list<ITransform const &>{ t }
		);
		// TODO - Remove
		s.render_objects_.back().transforms_.emplace_back(&t);
		s.render_objects_.back().texture_stride_ = 1;
		tr->SetEngineData(object_id);
	}

	// Add a mesh renderer component to the render pool
	void RenderPoolGL::AddMeshRenderer(ose::ITransform const & t, unowned_ptr<MeshRenderer> mr)
	{
		if(mr->GetMesh() == nullptr)
			return;

		// Each mesh has its own render object
		// TODO - Mesh renderers sharing a mesh will use the same render object
		// TODO - Should use glDrawElementsInstanced for rendering the shared meshes
		ShaderGroupGL & s = render_passes_[0].shader_groups_[1];

		// Get the mesh object to be rendered
		unowned_ptr<Mesh const> mesh { mr->GetMesh() };
		
		// Create a VBO for the render object
		GLuint vbo;
		glGenBuffers(1, &vbo);
		// Data consists of the vertex data is given in the mesh object
		// TODO - Include tangent, bitangent and any other required data
		std::vector<float> data(mesh->GetPositionData().size() + mesh->GetNormalData().size() + mesh->GetTexCoordData().size());
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		for(size_t p = 0, n = 0, t = 0; p < mesh->GetPositionData().size() && n < mesh->GetNormalData().size() && t < mesh->GetTexCoordData().size(); p += 3, n += 3, t += 2)
		{
			data[p + n + t + 0] = mesh->GetPositionData()[p + 0];
			data[p + n + t + 1] = mesh->GetPositionData()[p + 1];
			data[p + n + t + 2] = mesh->GetPositionData()[p + 2];

			data[p + n + t + 3] = mesh->GetNormalData()[n + 0];
			data[p + n + t + 4] = mesh->GetNormalData()[n + 1];
			data[p + n + t + 5] = mesh->GetNormalData()[n + 2];

			data[p + n + t + 6] = mesh->GetTexCoordData()[t + 0];
			data[p + n + t + 7] = mesh->GetTexCoordData()[t + 1];
		}
		glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(float), data.data(), GL_STATIC_DRAW);

		// Create an IBO for the render object
		GLuint ibo;
		glGenBuffers(1, &ibo);
		// Data consists of indices to vertices, where 3 consecutive indices make up a triangle
		std::vector<unsigned int> ibo_data;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		for(MeshSection section : mesh->GetSections())
		{
			for(unsigned int face_index : section.GetFaceIndices())
			{
				ibo_data.push_back(face_index);
			}
		}
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibo_data.size() * sizeof(unsigned int), ibo_data.data(), GL_STATIC_DRAW);

		// Create a VAO for the render object
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		// TODO - Vertex attrib locations are to be controlled by the built shader program
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(6 * sizeof(float)));
		// Unbind the vao
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Add a new render object
		GLenum primitive { GL_TRIANGLES };
		GLint first { 0 };
		GLint count { static_cast<GLint>(ibo_data.size()) };
		uint32_t object_id { NextComponentId() };
		s.render_objects_.emplace_back(
			std::initializer_list<uint32_t>{ object_id },
			ERenderObjectType::MESH_RENDERER,
			vbo, vao,
			primitive, first, count,
			std::initializer_list<GLuint>{  }
		//std::initializer_list<glm::mat4>{ t.GetTransformMatrix() }
		//std::initializer_list<ITransform const &>{ t }
		);

		// If the mesh renderer contains a material, add the first texture
		// TODO - Material determines shader group and can contain multiple textures
		if(mr->GetMaterial())
		{
			if(mr->GetMaterial()->GetTextures().size() > 0)
			{
				if(mr->GetMaterial()->GetTextures()[0])
				{
					s.render_objects_.back().textures_.emplace_back(static_cast<unowned_ptr<TextureGL const>>(mr->GetMaterial()->GetTextures()[0])->GetGlTexId());
				}
			}
		}

		// TODO - Remove
		s.render_objects_.back().ibo_ = ibo;
		s.render_objects_.back().transforms_.emplace_back(&t);
		s.render_objects_.back().texture_stride_ = 1; // TODO - Change to num textures in material
		mr->SetEngineData(object_id);
	}

	// Remove a sprite renderer component from the render pool
	void RenderPoolGL::RemoveSpriteRenderer(unowned_ptr<SpriteRenderer> sr)
	{
		// Try to find the render object the sprite renderer belongs to
		bool found { false };
		for(auto & p : render_passes_) {
			for(auto & s : p.shader_groups_) {
				for(auto it = s.render_objects_.begin(); it != s.render_objects_.end(); ++it) {
					if(it->type_ == ERenderObjectType::SPRITE_RENDERER)
					{
						// Find the sprite renderer data within the render object
						uint32_t object_id { std::any_cast<uint32_t>(sr->GetEngineData()) };
						for(int i = 0; i < it->component_ids_.size(); i++)
						{
							if(it->component_ids_[i] == object_id)
							{
								// Remove the component
								it->component_ids_.erase(it->component_ids_.begin() + i);
								it->transforms_.erase(it->transforms_.begin() + i);
								it->textures_.erase(it->textures_.begin() + i);
								found = true;
								break;
							}
						}
						// If there are no sprite renderers left in the render object, erase the render object
						if(it->component_ids_.size() == 0)
						{
							glDeleteBuffers(1, &it->vbo_);
							glDeleteVertexArrays(1, &it->vao_);
							s.render_objects_.erase(it);
						}
						// If the sprite renderer was found then exit the method early
						if(found)
						{
							return;
						}
					}
				}
			}
		}
	}

	// Remove a tile renderer component from the render pool
	void RenderPoolGL::RemoveTileRenderer(unowned_ptr<TileRenderer> tr)
	{
		// Try to find the render object the tile renderer belongs to
		for(auto & p : render_passes_) {
			for(auto & s : p.shader_groups_) {
				for(auto it = s.render_objects_.begin(); it != s.render_objects_.end(); ++it) {
					if(it->type_ == ERenderObjectType::TILE_RENDERER)
					{
						// Find the tile renderer data within the render object
						uint32_t object_id { std::any_cast<uint32_t>(tr->GetEngineData()) };
						
						// Can remove the entire render object since each tile renderer has its own render object
						if(it->component_ids_[0] == object_id)
						{
							glDeleteBuffers(1, &it->vbo_);
							glDeleteVertexArrays(1, &it->vao_);
							s.render_objects_.erase(it);
							return;
						}
					}
				}
			}
		}
	}

	// Remove a mesh renderer component from the render pool
	void RenderPoolGL::RemoveMeshRenderer(unowned_ptr<MeshRenderer> mr)
	{
		// Try to find the render object the mesh renderer belongs to
		for(auto & p : render_passes_) {
			for(auto & s : p.shader_groups_) {
				for(auto it = s.render_objects_.begin(); it != s.render_objects_.end(); ++it) {
					if(it->type_ == ERenderObjectType::MESH_RENDERER)
					{
						// Find the mesh renderer data within the render object
						uint32_t object_id { std::any_cast<uint32_t>(mr->GetEngineData()) };

						// Can remove the entire render object since each mesh renderer has its own render object
						// TODO - Change this once instancing is implemented
						if(it->component_ids_[0] == object_id)
						{
							glDeleteBuffers(1, &it->vbo_);
							glDeleteBuffers(1, &it->ibo_);
							glDeleteVertexArrays(1, &it->vao_);
							s.render_objects_.erase(it);
							return;
						}
					}
				}
			}
		}
	}
}
