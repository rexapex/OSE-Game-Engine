#pragma once
#include "OSE-Core/Types.h"

namespace ose
{
	class ITransform;
	class SpriteRenderer;
	class TileRenderer;
	class MeshRenderer;
	class PointLight;

	class RenderPool
	{
	public:
		RenderPool();
		virtual ~RenderPool();

		// Set the size of the framebuffer (required if render pool contains deferred shading render pass)
		virtual void SetFramebufferSize(int width, int height) = 0;

		// Add a sprite renderer component to the render pool
		virtual void AddSpriteRenderer(ITransform const & t, unowned_ptr<SpriteRenderer> sr) = 0;

		// Add a tile renderer component to the render pool
		virtual void AddTileRenderer(ITransform const & t, unowned_ptr<TileRenderer> tr) = 0;

		// Add a mesh renderer component to the render pool
		virtual void AddMeshRenderer(ITransform const & t, unowned_ptr<MeshRenderer> mr) = 0;

		// Add a point light component to the render pool
		virtual void AddPointLight(ITransform const & t, unowned_ptr<PointLight> pl) = 0;

		// Remove a sprite renderer component from the render pool
		virtual void RemoveSpriteRenderer(unowned_ptr<SpriteRenderer> sr) = 0;

		// Remove a tile renderer component from the render pool
		virtual void RemoveTileRenderer(unowned_ptr<TileRenderer> tr) = 0;

		// Remove a mesh renderer component from the render pool
		virtual void RemoveMeshRenderer(unowned_ptr<MeshRenderer> mr) = 0;

		// Remove a point light component from the render pool
		virtual void RemovePointLight(unowned_ptr<PointLight> pl) = 0;
	};
}

