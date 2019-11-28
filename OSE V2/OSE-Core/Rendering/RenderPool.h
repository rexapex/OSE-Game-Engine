#pragma once
#include "OSE-Core/Types.h"

namespace ose
{
	namespace math
	{
		class ITransform;
	}
	namespace entity
	{
		class SpriteRenderer;
	}
}

namespace ose::rendering
{
	class RenderPool
	{
	public:
		RenderPool();
		~RenderPool();

		// Add a sprite renderer component to the render pool
		virtual void AddSpriteRenderer(ose::math::ITransform const & t, ose::unowned_ptr<ose::entity::SpriteRenderer> sr) = 0;
	};
}

