#pragma once
#include "OSE-Core/Rendering/RenderPool.h"
#include "OSE-Core/Math/ITransform.h"
#include "OSE-Core/Math/Transform.h"

namespace ose::rendering
{
	class RenderPoolVK final : public RenderPool
	{
	public:
		RenderPoolVK();
		~RenderPoolVK();

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
	};
}

