#pragma once
#include "OSE-Core/Types.h"

namespace ose
{
	class ITransform;
	class SpriteRenderer;
	class TileRenderer;
	class MeshRenderer;
	class PointLight;
	class DirLight;

	class RenderPool
	{
	public:
		RenderPool();
		virtual ~RenderPool();

		// Set the size of the framebuffer (required if render pool contains deferred shading render pass)
		virtual void SetFramebufferSize(int width, int height) = 0;

		// Add a sprite renderer component to the render pool
		virtual void AddSpriteRenderer(ITransform const & t, SpriteRenderer * sr) = 0;

		// Add a tile renderer component to the render pool
		virtual void AddTileRenderer(ITransform const & t, TileRenderer * tr) = 0;

		// Add a mesh renderer component to the render pool
		virtual void AddMeshRenderer(ITransform const & t, MeshRenderer * mr) = 0;

		// Add a point light component to the render pool
		virtual void AddPointLight(ITransform const & t, PointLight * pl) = 0;

		// Add a direction light component to the render pool
		virtual void AddDirLight(ITransform const & t, DirLight * dl) = 0;

		// Remove a sprite renderer component from the render pool
		virtual void RemoveSpriteRenderer(SpriteRenderer * sr) = 0;

		// Remove a tile renderer component from the render pool
		virtual void RemoveTileRenderer(TileRenderer * tr) = 0;

		// Remove a mesh renderer component from the render pool
		virtual void RemoveMeshRenderer(MeshRenderer * mr) = 0;

		// Remove a point light component from the render pool
		virtual void RemovePointLight(PointLight * pl) = 0;

		// Remove a direction light component from the render pool
		virtual void RemoveDirLight(DirLight * dl) = 0;
	};
}

