#pragma once
#include "OSE-Core/Types.h"
#include "OSE-Core/Entity/SpriteRenderer.h"
#include "OSE-Core/Math/ITransform.h"

namespace ose::rendering
{
	class RenderPool
	{
	public:
		RenderPool();
		~RenderPool();

		// Add a sprite renderer component to the render pool
		virtual void AddSpriteRenderer(math::ITransform const & t, ose::unowned_ptr<entity::SpriteRenderer> sr) = 0;
	};
}

