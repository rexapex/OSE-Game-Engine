#pragma once
#include "SpriteRenderer.h"

namespace ose::rendering
{
	struct SpriteRendererGL : public SpriteRenderer
	{
		SpriteRendererGL(const std::string & name) : SpriteRenderer(name) {}
	};
}
