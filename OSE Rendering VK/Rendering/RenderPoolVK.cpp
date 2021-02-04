#include "pch.h"
#include "RenderPoolVK.h"

namespace ose::rendering
{
	RenderPoolVK::RenderPoolVK()
	{
	
	}

	RenderPoolVK::~RenderPoolVK()
	{
	
	}

	// Initialise the render pool
	void RenderPoolVK::Init(int fbwidth, int fbheight)
	{
	
	}

	// Set the size of the framebuffer (required if render pool contains deferred shading render pass)
	void RenderPoolVK::SetFramebufferSize(int width, int height)
	{
	
	}

	// Add a sprite renderer component to the render pool
	void RenderPoolVK::AddSpriteRenderer(ose::ITransform const & t, SpriteRenderer * sr)
	{
	
	}

	// Add a tile renderer component to the render pool
	void RenderPoolVK::AddTileRenderer(ose::ITransform const & t, TileRenderer * tr)
	{
	
	}

	// Add a mesh renderer component to the render pool
	void RenderPoolVK::AddMeshRenderer(ose::ITransform const & t, MeshRenderer * mr)
	{
	
	}

	// Add a point light component to the render pool
	void RenderPoolVK::AddPointLight(ITransform const & t, PointLight * pl)
	{
	
	}

	// Add a direction light component to the render pool
	void RenderPoolVK::AddDirLight(ITransform const & t, DirLight * dl)
	{
	
	}

	// Remove a sprite renderer component from the render pool
	void RenderPoolVK::RemoveSpriteRenderer(SpriteRenderer * sr)
	{
	
	}

	// Remove a tile renderer component from the render pool
	void RenderPoolVK::RemoveTileRenderer(TileRenderer * tr)
	{
	
	}

	// Remove a mesh renderer component from the render pool
	void RenderPoolVK::RemoveMeshRenderer(MeshRenderer * mr)
	{
	
	}

	// Remove a point light component from the render pool
	void RenderPoolVK::RemovePointLight(PointLight * pl)
	{
	
	}

	// Remove a direction light component from the render pool
	void RenderPoolVK::RemoveDirLight(DirLight * dl)
	{
	
	}
}
