#pragma once

namespace ose::rendering
{
	struct TextureAtlasSlot
	{
		int32_t x_;
		int32_t y_;
		int32_t w_;
		int32_t h_;

		TextureAtlasSlot(int32_t x, int32_t y, int32_t w, int32_t h) : x_(x), y_(y), w_(w), h_(h) {}
	};
}
