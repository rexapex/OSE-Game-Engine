#include "pch.h"
#include "RenderPoolGL.h"
#include "ERenderObjectType.h"

#include "OSE-Core/Entity/Component/SpriteRenderer.h"
#include "OSE-Core/Entity/Component/TileRenderer.h"
#include "OSE-Core/Entity/Component/MeshRenderer.h"

#include "OSE-Core/Entity/Component/PointLight.h"
#include "OSE-Core/Entity/Component/DirLight.h"

#include "OSE-Core/Resources/Tilemap/Tilemap.h"

#include "OSE-Core/Resources/Texture/Texture.h"
#include "Atlas/TextureAtlas.h"
#include "Texture/Texture2DGL.h"

// TODO - Remove
#include "OSE-Core/Math/ITransform.h"
#include "Shader/ShaderProgGLSL.h"
#include "Shader/Shaders/BRDFShaderProgGLSL.h"
#include "Shader/Shaders/Default2DShaderProgGLSL.h"
#include "Shader/Shaders/Default3DShaderProgGLSL.h"

namespace ose::rendering
{
	RenderPoolGL::RenderPoolGL()
	{
		
	}

	RenderPoolGL::~RenderPoolGL()
	{
		for(auto const & render_pass : render_passes_)
		{
			for(auto const & material_group : render_pass.material_groups_)
			{
				glDeleteProgram(material_group.shader_prog_);

				for(auto const & render_group : material_group.render_groups_)
				{
					glDeleteBuffers(1, &render_group.vbo_);
					glDeleteVertexArrays(1, &render_group.vao_);
				}
			}
		}
	}

	// Initialise the render pool
	void RenderPoolGL::Init(int fbwidth, int fbheight)
	{
		/*framebuffers_.emplace_back(fbwidth, fbheight);
		auto & fb = framebuffers_.back();

		// Create a deferred shading render pass
		{
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

			// Create the deferred rendering shader
			deferred_shader_prog_ = ose::make_unique<shader::BRDFShaderProgGLSL>();
			render_passes_.emplace_back();
			ShaderGroupGL s;
			s.shader_prog_ = deferred_shader_prog_->GetShaderProgId();
			//render_passes_.back().clear_ = GL_COLOR_BUFFER_BIT;	// TODO - Try this with GL_DEPTH_TEST disabled

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
					fb.GetPosBuffer(),
					fb.GetNormBuffer(),
					fb.GetColBuffer()
				}
				//std::initializer_list<glm::mat4>{ t.GetTransformMatrix() }
				//std::initializer_list<ITransform const &>{ t }
			);
			// TODO - Remove
			s.render_objects_.back().transforms_.emplace_back(&deferred_shader_transform_);
			s.render_objects_.back().texture_stride_ = 3;
			render_passes_.back().shader_groups_.push_back(s);
		}*/

		// Insert a render pass before the deferred render pass to render opaque objects to
		//render_passes_.insert(render_passes_.begin(), RenderPassGL{});
		//render_passes_[0].fbo_ = fb.GetFbo();
		render_passes_.emplace_back();
		render_passes_[0].clear_ = true;
		render_passes_[0].clear_mode_ = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
		render_passes_[0].enable_depth_test_ = true;
		render_passes_[0].depth_func_ = GL_LEQUAL;

		// TODO - Remove
		//{
		//	// Create the default 2d shader prog
		//	default_2d_shader_prog_ = ose::make_unique<shader::Default2DShaderProgGLSL>();
		//	default_2d_shader_prog_->CreateShaderProg();
		//	ShaderGroupGL sg;
		//	sg.enable_blend_ = false;
		//	sg.shader_prog_ = default_2d_shader_prog_->GetShaderProgId();
		//	render_passes_[0].shader_groups_.push_back(sg);

		//	// Create the default transparency 2d shader prog
		//	sg.enable_blend_ = true;
		//	sg.blend_fac_ = GL_SRC_ALPHA;
		//	sg.blend_func_ = GL_ONE_MINUS_SRC_ALPHA;
		//	render_passes_[0].shader_groups_.push_back(sg);
		//}

		//{
		//	// Create the default brdf 3d shader prog
		//	brdf_shader_prog_ = ose::make_unique<shader::BRDFShaderProgGLSL>();
		//	brdf_shader_prog_->CreateShaderProg();
		//	ShaderGroupGL sg;
		//	sg.shader_prog_ = brdf_shader_prog_->GetShaderProgId();
		//	render_passes_[0].shader_groups_.push_back(sg);
		//}

		// Insert a render pass before the deferred render pass to render (semi)transparent objects to
		//render_passes_.emplace_back();
		//render_passes_[1].enable_blend_ = true;
		//render_passes_[1].blend_fac_ = GL_SRC_ALPHA;
		//render_passes_[1].blend_func_ = GL_ONE_MINUS_SRC_ALPHA;
		//render_passes_[1].enable_depth_test_ = true;
		//render_passes_[1].depth_func_ = GL_LEQUAL;

		//// TODO - Remove
		//{
		//	// TODO - Reuse one from above
		//	// Create the default 2d shader prog
		//	default_2d_shader_prog_ = ose::make_unique<shader::Default2DShaderProgGLSL>();
		//	ShaderGroupGL sg;
		//	sg.shader_prog_ = default_2d_shader_prog_->GetShaderProgId();
		//	render_passes_[1].shader_groups_.push_back(sg);
		//}

		// TODO - Remove
		/*{
			// Create the default 3d shader prog
			default_3d_shader_prog_ = ose::make_unique<shader::Default3DShaderProgGLSL>();
			ShaderGroupGL sg;
			sg.shader_prog_ = default_3d_shader_prog_->GetShaderProgId();
			render_passes_[0].shader_groups_.push_back(sg);
		}*/
	}

	// Set the size of the framebuffer (required if render pool contains deferred shading render pass)
	void RenderPoolGL::SetFramebufferSize(int width, int height)
	{
		// Update the deferred shading framebuffer objects
		for(auto & fb : framebuffers_)
		{
			fb.Resize(width, height);
		}
	}

	// Add a sprite renderer component to the render pool
	void RenderPoolGL::AddSpriteRenderer(ITransform const & t, SpriteRenderer * sr)
	{
		if(sr->GetTexture() == nullptr || sr->GetMaterial() == nullptr)
		{
			LOG_ERROR("Failed to add sprite renderer, texture or material are nullptr");
			return;
		}

		// Get the material group to add the sprite renderer to
		MaterialGroupGL * material_group { GetMaterialGroup(render_passes_[0], sr->GetMaterial()) };
		if(!material_group)
			return;

		// Get the render group to add the sprite renderer to
		std::vector<Texture const &> textures { *sr->GetTexture() };
		RenderGroupGL * render_group { GetRenderGroup(*material_group, textures, true) };

		float w = static_cast<float>(sr->GetTexture()->GetWidth());
		float h = static_cast<float>(sr->GetTexture()->GetHeight());

		// Append the data for this sprite to the vbo
		// Data consists of 2-float position and 2-float tex coords interleaved
		float data[] = {
			0, h, 0, 0,
			w, 0, 1, 1,
			w, h, 1, 0,

			w, 0, 1, 1,
			0, h, 0, 0,
			0, 0, 0, 1
		};
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

		// TODO - Remove
		material_group->render_groups_.back().transforms_.emplace_back(&t);
		material_group->render_groups_.back().texture_stride_ = 1;
		sr->SetEngineData(object_id);
	}

	// Add a tile renderer component to the render pool
	void RenderPoolGL::AddTileRenderer(ITransform const & t, TileRenderer * tr)
	{
		if(tr->GetTexture() == nullptr || tr->GetTilemap() == nullptr || tr->GetMaterial() == nullptr)
		{
			LOG_ERROR("Failed to add tile renderer, texture, tilemap or material are nullptr");
			return;
		}

		// Get the material group to add the sprite renderer to
		MaterialGroupGL * material_group = GetMaterialGroup(render_passes_[0], tr->GetMaterial());
		if(!material_group)
			return;

		// Get a reference to the tilemap
		auto & tilemap = *tr->GetTilemap();

		// Calculate tile dimensions s.t. when multiplied by the texture dimensions in the shader, the tiles will be the correct size
		float tile_width  { static_cast<float>(tr->GetTexture()->GetWidth()) / tr->GetNumCols() };
		float tile_height { static_cast<float>(tr->GetTexture()->GetHeight()) / tr->GetNumRows() };

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

		// Unlike SpriteRenderer, TileRenderer cannot share a render group since the tile grid is encoded into the buffer data
		// Add a new render object
		GLenum primitive { GL_TRIANGLES };
		GLint first { 0 };
		GLint count { 6 * tilemap_width * tilemap_height };
		uint32_t object_id { NextComponentId() };
		material_group->render_groups_.emplace_back(
			std::initializer_list<uint32_t>{ object_id },
			ERenderObjectType::TILE_RENDERER,
			vbo, vao,
			primitive, first, count
			//std::initializer_list<GLuint>{ static_cast<TextureGL const *>(tr->GetTexture())->GetGlTexId() }
			//std::initializer_list<glm::mat4>{ t.GetTransformMatrix() }
			//std::initializer_list<ITransform const &>{ t }
		);
		// TODO - Remove
		material_group->render_groups_.back().transforms_.emplace_back(&t);
		material_group->render_groups_.back().texture_stride_ = 1;
		tr->SetEngineData(object_id);
	}

	// Add a mesh renderer component to the render pool
	void RenderPoolGL::AddMeshRenderer(ose::ITransform const & t, MeshRenderer * mr)
	{
		if(mr->GetMesh() == nullptr || mr->GetMaterial() == nullptr)
			return;

		//MaterialGroupGL & s = render_passes_[0].material_groups_[1];
		MaterialGroupGL * material_group = GetMaterialGroup(render_passes_[0], mr->GetMaterial());
		if(!material_group)
			return;

		// Get the mesh object to be rendered
		Mesh const * mesh { mr->GetMesh() };
		
		// Create a VBO for the render object
		GLuint vbo;
		glGenBuffers(1, &vbo);
		// Data consists of the vertex data is given in the mesh object
		// TODO - Include tangent, bitangent and any other required data
		std::vector<float> data(mesh->GetPositionData().size() + mesh->GetNormalData().size() + mesh->GetTexCoordData().size() + mesh->GetTangentData().size());
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		for(size_t p = 0, n = 0, t = 0, tan = 0; p < mesh->GetPositionData().size() && n < mesh->GetNormalData().size()
			&& t < mesh->GetTexCoordData().size() && tan < mesh->GetTangentData().size(); p += 3, n += 3, t += 2, tan += 3)
		{
			data[p + n + t + tan + 0] = mesh->GetPositionData()[p + 0];
			data[p + n + t + tan + 1] = mesh->GetPositionData()[p + 1];
			data[p + n + t + tan + 2] = mesh->GetPositionData()[p + 2];

			data[p + n + t + tan + 3] = mesh->GetNormalData()[n + 0];
			data[p + n + t + tan + 4] = mesh->GetNormalData()[n + 1];
			data[p + n + t + tan + 5] = mesh->GetNormalData()[n + 2];

			data[p + n + t + tan + 6] = mesh->GetTexCoordData()[t + 0];
			data[p + n + t + tan + 7] = mesh->GetTexCoordData()[t + 1];

			data[p + n + t + tan + 8] = mesh->GetTangentData()[tan + 0];
			data[p + n + t + tan + 9] = mesh->GetTangentData()[tan + 1];
			data[p + n + t + tan + 10] = mesh->GetTangentData()[tan + 2];
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (GLvoid*)(6 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (GLvoid*)(8 * sizeof(float)));
		// Unbind the vao
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Add a new render object
		GLenum primitive { GL_TRIANGLES };
		GLint first { 0 };
		GLint count { static_cast<GLint>(ibo_data.size()) };
		uint32_t object_id { NextComponentId() };
		material_group->render_groups_.emplace_back(
			std::initializer_list<uint32_t>{ object_id },
			ERenderObjectType::MESH_RENDERER,
			vbo, vao,
			primitive, first, count
			//std::initializer_list<GLuint>{  }
		//std::initializer_list<glm::mat4>{ t.GetTransformMatrix() }
		//std::initializer_list<ITransform const &>{ t }
		);

		// If the mesh renderer contains a material, add the first texture
		// TODO - Material determines shader group and can contain multiple textures
		GLuint texture_stride { 0 };
		if(mr->GetMaterial())
		{
			for(auto texture : mr->GetMaterial()->GetTextures())
			{
				if(texture)
				{
					material_group->render_groups_.back().textures_.emplace_back(static_cast<TextureGL const *>(texture)->GetGlTexId());
					++texture_stride;
				}
			}
		}


		// Each mesh has its own render group
		// TODO - Mesh renderers sharing a mesh will use the same render group
		// TODO - Should use glDrawElementsInstanced for rendering the shared meshes
		material_group->render_groups_.back().ibo_ = ibo;
		material_group->render_groups_.back().transforms_.emplace_back(&t);
		material_group->render_groups_.back().texture_stride_ = texture_stride;
		mr->SetEngineData(object_id);
	}

	// Add a point light component to the render pool
	void RenderPoolGL::AddPointLight(ITransform const & t, PointLight * pl)
	{
		// TODO - Update the position of the light data when the point light's entity moves

		PointLightData data;
		data.position_ = glm::vec3(t.GetTranslation());
		data.color_ = glm::vec3(pl->GetColor());
		point_lights_.push_back(data);
	}

	// Add a direction light component to the render pool
	void RenderPoolGL::AddDirLight(ITransform const & t, DirLight * dl)
	{
		// TODO - Update the position of the light data when the point light's entity rotates

		DirLightData data;
		data.direction_ = glm::vec3(t.GetForward());
		data.color_ = glm::vec3(dl->GetColor());
		dir_lights_.push_back(data);
	}

	// Remove a sprite renderer component from the render pool
	void RenderPoolGL::RemoveSpriteRenderer(SpriteRenderer * sr)
	{
		// Try to find the render object the sprite renderer belongs to
		bool found { false };
		for(auto & p : render_passes_) {
			for(auto & s : p.material_groups_) {
				for(auto it = s.render_groups_.begin(); it != s.render_groups_.end(); ++it) {
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
							s.render_groups_.erase(it);
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
	void RenderPoolGL::RemoveTileRenderer(TileRenderer * tr)
	{
		// Try to find the render object the tile renderer belongs to
		for(auto & p : render_passes_) {
			for(auto & s : p.material_groups_) {
				for(auto it = s.render_groups_.begin(); it != s.render_groups_.end(); ++it) {
					if(it->type_ == ERenderObjectType::TILE_RENDERER)
					{
						// Find the tile renderer data within the render object
						uint32_t object_id { std::any_cast<uint32_t>(tr->GetEngineData()) };
						
						// Can remove the entire render object since each tile renderer has its own render object
						if(it->component_ids_[0] == object_id)
						{
							glDeleteBuffers(1, &it->vbo_);
							glDeleteVertexArrays(1, &it->vao_);
							s.render_groups_.erase(it);
							return;
						}
					}
				}
			}
		}
	}

	// Remove a mesh renderer component from the render pool
	void RenderPoolGL::RemoveMeshRenderer(MeshRenderer * mr)
	{
		// Try to find the render object the mesh renderer belongs to
		for(auto & p : render_passes_) {
			for(auto & s : p.material_groups_) {
				for(auto it = s.render_groups_.begin(); it != s.render_groups_.end(); ++it) {
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
							s.render_groups_.erase(it);
							return;
						}
					}
				}
			}
		}
	}

	// Remove a point light component from the render pool
	void RenderPoolGL::RemovePointLight(PointLight * pl)
	{
		// TODO
	}

	// Remove a direction light component from the render pool
	void RenderPoolGL::RemoveDirLight(DirLight * dl)
	{
		// TODO
	}

	// Get a material group to render the given material in
	// If no suitable material group exists, a new group is created
	MaterialGroupGL * RenderPoolGL::GetMaterialGroup(RenderPassGL & render_pass, Material const * material)
	{
		// Returns true if a material group's blending setup matches an EBlendMode object
		auto is_blending_correct = [](EBlendMode mode, MaterialGroupGL const & group) -> bool {
			if(mode == EBlendMode::OPAQUE && group.enable_blend_ == false)
				return true;
			if(mode == EBlendMode::ONE_MINUS_SRC_ALPHA && group.enable_blend_ == true && group.blend_func_ == GL_ONE_MINUS_SRC_ALPHA)
				return true;
			return false;
		};

		// Try to find a material group to add the material to
		MaterialGroupGL * material_group { nullptr };
		shader::ShaderProgGLSL const * shader_prog = dynamic_cast<shader::ShaderProgGLSL const *>(material->GetShaderProg());
		if(shader_prog)
		{
			for(auto & s : render_pass.material_groups_)
			{
				if(shader_prog->GetShaderProgId() == s.shader_prog_ && is_blending_correct(material->GetBlendMode(), s))
					material_group = &s;
			}
		}
		else
		{
			LOG_ERROR("Failed to find a suitable shader group, material shader is not of type ShaderProgGLSL");
			return nullptr;
		}

		// If no usable material group exists, create a new one
		if(!material_group)
		{
			MaterialGroupGL mg;
			mg.enable_blend_ = material->GetBlendMode() == EBlendMode::OPAQUE ? false : true;
			mg.blend_fac_ = GL_SRC_ALPHA;
			mg.blend_func_ = GL_ONE_MINUS_SRC_ALPHA;
			mg.shader_prog_ = shader_prog->GetShaderProgId();

			// Rendering of opaque objects should be done before rendering of alpha enabled objects
			// Therefore, if attempting to add an opaque shader group, ensure it is inserted before any alpha shader groups
			if(material->GetBlendMode() == EBlendMode::OPAQUE)
			{
				material_group = &*render_pass.material_groups_.insert(render_pass.material_groups_.begin(), mg);
			}
			else
			{
				render_pass.material_groups_.push_back(mg);
				material_group = &render_pass.material_groups_.back();
			}
		}

		return material_group;
	}

	// Get a render group for rendering the given object
	// If no suitable render group exists, a new group is created
	// @param is_static refers to the entities static property (i.e. whether its transform will remain constant)
	RenderGroupGL * RenderPoolGL::GetRenderGroup(MaterialGroupGL & material_group, std::vector<Texture const &> const & textures, bool is_static)
	{
		// Try to find a render group to add the object to
		RenderGroupGL * render_group { nullptr };
		for(auto & g : material_group.render_groups_)
		{
			if(g.texture_atlas_->TryAddTextures(textures))
			{
				render_group = &g;
				break;
			}
		}

		// If no usable render group exists, create a new one
		if(!render_group)
		{
			// Create a VBO for the render group
			GLuint vbo;
			glGenBuffers(1, &vbo);

			// Create a VAO for the render group
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

			// Create the new render group
			GLenum primitive { GL_TRIANGLES };
			GLint first { 0 };
			GLint count { 6 };
			uint32_t object_id { NextComponentId() };
			material_group.render_groups_.emplace_back(
				std::initializer_list<uint32_t>{ object_id },
				ERenderObjectType::SPRITE_RENDERER,
				vbo, vao,
				GL_TRIANGLES, first, count
			);
			render_group = &material_group.render_groups_.back();

			// Create a texture atlas for the render group
			texture_atlases_.push_back(std::make_unique<TextureAtlas>(2048 * 1024, 2048 * 1024, 3));
			render_group->texture_atlas_ = texture_atlases_.back().get();
		}

		return render_group;
	}
}
