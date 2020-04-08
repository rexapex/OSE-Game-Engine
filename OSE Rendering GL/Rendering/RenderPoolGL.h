#pragma once
#include "OSE-Core/Rendering/RenderPool.h"
#include "OSE-Core/Math/ITransform.h"
#include "RenderPassGL.h"
#include "OSE-Core/Math/Transform.h"
#include "Lights/PointLightData.h"

namespace ose::shader
{
	class BRDFDeferredShaderProgGLSL;
	class Default2DShaderProgGLSL;
	class Default3DShaderProgGLSL;
}

namespace ose::rendering
{
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
		void AddSpriteRenderer(ose::ITransform const & t, unowned_ptr<SpriteRenderer> sr) override;

		// Add a tile renderer component to the render pool
		void AddTileRenderer(ose::ITransform const & t, unowned_ptr<TileRenderer> tr) override;

		// Add a mesh renderer component to the render pool
		void AddMeshRenderer(ose::ITransform const & t, unowned_ptr<MeshRenderer> mr) override;

		// Add a point light component to the render pool
		void AddPointLight(ITransform const & t, unowned_ptr<PointLight> pl) override;

		// Remove a sprite renderer component from the render pool
		void RemoveSpriteRenderer(unowned_ptr<SpriteRenderer> sr) override;

		// Remove a tile renderer component from the render pool
		void RemoveTileRenderer(unowned_ptr<TileRenderer> tr) override;

		// Remove a mesh renderer component from the render pool
		void RemoveMeshRenderer(unowned_ptr<MeshRenderer> mr) override;

		// Remove a point light component from the render pool
		void RemovePointLight(unowned_ptr<PointLight> pl) override;

		// Get the list of render passes s.t. they can be rendered by the rendering engine
		std::vector<RenderPassGL> const & GetRenderPasses() const { return render_passes_; }

		// Get the list of point lights s.t. they can be rendered by the rendering engine
		std::vector<PointLightData> const & GetPointLights() const { return point_lights_; }

	private:
		// List of all render passes the render pool is to perform on each rendering engine update
		std::vector<RenderPassGL> render_passes_;

		// List of all dynamic point lights
		std::vector<PointLightData> point_lights_;

		// Dummy transform used by deferred shaders
		Transform deferred_shader_transform_;

		// Default shader programs
		std::unique_ptr<shader::BRDFDeferredShaderProgGLSL> deferred_shader_prog_;
		std::unique_ptr<shader::Default2DShaderProgGLSL> default_2d_shader_prog_;
		std::unique_ptr<shader::Default3DShaderProgGLSL> default_3d_shader_prog_;

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

