#pragma once

#include "OSE-Core/Engine/EngineTask.h"

namespace ose::entity
{
	class SpriteRenderer;
}

namespace ose::rendering
{
	using namespace entity;
	using namespace engine;

	class RenderTask : public EngineTask
	{
	public:
		// initialise the render object to render sprite renderer data
		virtual void init(SpriteRenderer * sprite_renderer) = 0;
	};
}
