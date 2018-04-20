#pragma once
#include "SpriteRenderer.h"

namespace origami_sheep_engine
{
	struct SpriteRendererGL : public SpriteRenderer
	{
		SpriteRendererGL(const std::string & name) : SpriteRenderer(name) {}
	};
}
