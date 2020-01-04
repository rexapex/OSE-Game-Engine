#pragma once
#include "OSE-Core/Types.h"
#include "Component.h"
#include "OSE-Core/Resources/Texture/Texture.h"
#include "OSE-Core/Resources/Tilemap/Tilemap.h"

namespace ose
{
	// Renders a 2D grid of tile textures at the entities transform
	class TileRenderer : public Component
	{
		// Declare TileRenderer as an OSE Component
		COMPONENT(TileRenderer, Component)
		
	private:
		// The texture to use as the atlas
		// NOTE - This pointer is owned and managed by the resource manager
		unowned_ptr<Texture const> texture_;

		// The tilemap to render to the screen
		// NOTE - This pointer is owned and managed by the resource manager
		unowned_ptr<Tilemap const> tilemap_;

		// The number of columns of tiles in the texture
		int32_t num_cols_ { 1 };

		// The number of rows of tiles in the texture
		int32_t num_rows_ { 1 };

		// The total numbers of tiles in the texture (not all rows/cols may be full)
		int32_t num_tiles_ { 1 };

		// The spacing between tiles (defaults to one - i.e. 1.0 * tile dimensions)
		float spacing_x_ { 1.0f };
		float spacing_y_ { 1.0f };

	public:

		unowned_ptr<Texture const> GetTexture() const { return texture_; }
		unowned_ptr<Tilemap const> GetTilemap() const { return tilemap_; }

		int32_t GetNumCols() const { return num_cols_; }
		int32_t GetNumRows() const { return num_rows_; }
		int32_t GetNumTiles() const { return num_tiles_; }

		float GetSpacingX() const { return spacing_x_; }
		float GetSpacingY() const { return spacing_y_; }

		void SetTexture(unowned_ptr<Texture const> texture) { texture_ = texture; }
		void SetTilemap(unowned_ptr<Tilemap const> tilemap) { tilemap_ = tilemap; }

		void SetNumCols(int32_t n) { if(n > 0) num_cols_ = n; }
		void SetNumRows(int32_t n) { if(n > 0) num_rows_ = n; }
		void SetNumTiles(int32_t n) { if(n > 0) num_tiles_ = n; }

		void SetSpacingX(float val) { if(val > 0) spacing_x_ = val; }
		void SetSpacingY(float val) { if(val > 0) spacing_y_ = val; }

		// Initialise the tile renderer
		TileRenderer(std::string const & name, unowned_ptr<Texture const> t, unowned_ptr<Tilemap const> tm,
			int32_t num_cols, int32_t num_rows, int32_t num_tiles, float spacing_x, float spacing_y)
			: Component(name), texture_(t), tilemap_(tm)
		{
			SetNumCols(num_cols);
			SetNumRows(num_rows);
			SetNumTiles(num_tiles);
			SetSpacingX(spacing_x);
			SetSpacingY(spacing_y);
		}

		// Does nothing
		virtual ~TileRenderer() noexcept {}

		// Default copy/move constructors
		TileRenderer(TileRenderer const & other) noexcept = default;
		TileRenderer & operator=(TileRenderer const & other) noexcept = default;
		TileRenderer(TileRenderer && other) noexcept = default;
		TileRenderer & operator=(TileRenderer && other) noexcept = default;
	};
}
