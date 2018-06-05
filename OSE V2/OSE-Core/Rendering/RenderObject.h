#pragma once

namespace ose::entity
{
	class SpriteRenderer;
}

namespace ose::rendering
{
	using namespace entity;

	class RenderObject
	{
	public:
		// initialise the render object to render sprite renderer data
		virtual void init(SpriteRenderer * sprite_renderer) = 0;
	};
}
