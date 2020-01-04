#pragma once
#include "OSE-Core/Types.h"

namespace ose
{
	class ITransform;
	class SpriteRenderer;
	class TileRenderer;

	class RenderPool
	{
	public:
		RenderPool();
		virtual ~RenderPool();

		// Add a sprite renderer component to the render pool
		virtual void AddSpriteRenderer(ITransform const & t, unowned_ptr<SpriteRenderer> sr) = 0;

		// Add a tile renderer component to the render pool
		virtual void AddTileRenderer(ITransform const & t, unowned_ptr<TileRenderer> tr) = 0;
	};
}

