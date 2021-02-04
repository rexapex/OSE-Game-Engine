#pragma once

namespace ose::resources
{
	class Texture;
}

namespace ose::rendering
{
	struct OccupiedTextureAtlasSlot
	{
		int32_t x_;
		int32_t y_;
		Texture const & texture_;

		OccupiedTextureAtlasSlot(int32_t x, int32_t y, Texture const & t) : x_(x), y_(y), texture_(t) {}
	};
}
