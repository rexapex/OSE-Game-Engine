#pragma once
#include "OSE-Core/Types.h"

namespace ose::resources
{
	class Tilemap
	{
	public:
		Tilemap(const std::string & name, const std::string & path, int32_t width, int32_t height);
		~Tilemap();
		// Copying is not allowed (same as ResourceManager)
		Tilemap(Tilemap &) = delete;
		Tilemap & operator=(Tilemap &) = delete;
		// Moving is allowed (same as ResourceManager)
		Tilemap(Tilemap &&) noexcept;
		Tilemap & operator=(Tilemap &&) noexcept;

		ose::unowned_ptr<int32_t> const GetTileGrid() const { return tile_grid_; }

		int32_t GetWidth() const { return width_; }
		int32_t GetHeight() const { return height_; }

		int32_t GetSpacingX() const { return spacing_x_; }
		int32_t GetSpacingY() const { return spacing_y_; }

		void SetSpacingX(int32_t val) { spacing_x_ = val; }
		void SetSpacingY(int32_t val) { spacing_y_ = val; }

		int32_t & operator[](size_t index) { return tile_grid_[index]; }
		int32_t & operator()(size_t row, size_t col) { return tile_grid_[col + row*width_]; }

	protected:
		std::string name_;
		std::string path_;

		int32_t * tile_grid_;

		int32_t width_;
		int32_t height_;

		int32_t spacing_x_;
		int32_t spacing_y_;
	};
}
