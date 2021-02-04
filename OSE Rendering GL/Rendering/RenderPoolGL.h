#pragma once
#include "OSE-Core/Rendering/RenderPool.h"
#include "OSE-Core/Math/ITransform.h"
#include "RenderPassGL.h"
#include "FramebufferGL.h"
#include "OSE-Core/Math/Transform.h"
#include "Lights/PointLightData.h"
#include "Lights/DirLightData.h"

namespace ose
{
	class Material;
}

namespace ose::shader
{
	class BRDFShaderProgGLSL;
	class Default2DShaderProgGLSL;
	class Default3DShaderProgGLSL;
}

namespace ose::rendering
{
	class TextureAtlas;

	class RenderPoolGL final : public RenderPool
	{
	public:
		RenderPoolGL();
		~RenderPoolGL();

		// Initialise the render pool
		void Init(int fbwidth, int fbheight);

		// Set the size of the framebuffer (required if render pool contains deferred shading render pass)
		void SetFramebufferSize(int width, int height) override;

		// Add a sprite renderer component to the render pool
		void AddSpriteRenderer(ose::ITransform const & t, SpriteRenderer * sr) override;

		// Add a tile renderer component to the render pool
		void AddTileRenderer(ose::ITransform const & t, TileRenderer * tr) override;

		// Add a mesh renderer component to the render pool
		void AddMeshRenderer(ose::ITransform const & t, MeshRenderer * mr) override;

		// Add a point light component to the render pool
		void AddPointLight(ITransform const & t, PointLight * pl) override;

		// Add a direction light component to the render pool
		void AddDirLight(ITransform const & t, DirLight * dl) override;

		// Remove a sprite renderer component from the render pool
		void RemoveSpriteRenderer(SpriteRenderer * sr) override;

		// Remove a tile renderer component from the render pool
		void RemoveTileRenderer(TileRenderer * tr) override;

		// Remove a mesh renderer component from the render pool
		void RemoveMeshRenderer(MeshRenderer * mr) override;

		// Remove a point light component from the render pool
		void RemovePointLight(PointLight * pl) override;

		// Remove a direction light component from the render pool
		void RemoveDirLight(DirLight * dl) override;

		// Get the list of render passes s.t. they can be rendered by the rendering engine
		std::vector<RenderPassGL> const & GetRenderPasses() const { return render_passes_; }

		// Get the list of point lights s.t. they can be rendered by the rendering engine
		std::vector<PointLightData> const & GetPointLights() const { return point_lights_; }

		// Get the list of direction lights s.t. they can be rendered by the rendering engine
		std::vector<DirLightData> const & GetDirLights() const { return dir_lights_; }

	private:
		// Get a material group to render the given material in
		// If no suitable material group exists, a new group is created
		MaterialGroupGL * GetMaterialGroup(RenderPassGL & render_pass, Material const * material);

		// Get a render group for rendering the given object
		// If no suitable render group exists, a new group is created
		// @param is_static refers to the entities static property (i.e. whether its transform will remain constant)
		RenderGroupGL * GetRenderGroup(MaterialGroupGL & material_group, std::vector<Texture const *> const & textures, bool is_static);

	private:
		// List of all render passes the render pool is to perform on each rendering engine update
		std::vector<RenderPassGL> render_passes_;

		// List of all dynamic point lights
		std::vector<PointLightData> point_lights_;

		// List of all dynamic direction lights
		std::vector<DirLightData> dir_lights_;

		// Dummy transform used by deferred shaders
		Transform deferred_shader_transform_;

		// Default shader programs
		uptr<shader::BRDFShaderProgGLSL> brdf_shader_prog_;
		uptr<shader::Default2DShaderProgGLSL> default_2d_shader_prog_;
		uptr<shader::Default3DShaderProgGLSL> default_3d_shader_prog_;

		// List of all active framebuffer objects used for deferred rendering
		std::vector<FramebufferGL> framebuffers_;

		// List of all available texture atlases
		std::vector<std::unique_ptr<TextureAtlas>> texture_atlases_;

		// Returns the next available object id
		static uint32_t NextComponentId()
		{
			static uint32_t id { 0 };
			return id++;
			// TODO - Test whether out of ids, if ran out should reset all ids and notify entity components
			// TODO - Alternatively use 64-bit integer which is unlikely to ever reach max value (typedef id)
		}
	};
}

