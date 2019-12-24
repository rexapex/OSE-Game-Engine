#pragma once
#include "OSE-Core/Types.h"
#include "Component.h"
#include "OSE-Core/Resources/Texture/Texture.h"
#include "OSE-Core/Resources/Tilemap/Tilemap.h"

namespace ose::entity
{
	// Renders a 2D grid of tile textures at the entities transform
	class TileRenderer : public Component
	{
		// Declare TileRenderer as an OSE Component
		COMPONENT(TileRenderer, Component)
		
	private:
		// The texture to use as the atlas
		// NOTE - This pointer is owned and managed by the resource manager
		ose::unowned_ptr<resources::Texture const> texture_;

		// The tilemap to render to the screen
		// NOTE - This pointer is owned and managed by the resource manager
		ose::unowned_ptr<resources::Tilemap const> tilemap_;

		// The number of columns of tiles in the texture
		int32_t num_cols_ { 1 };

		// The number of rows of tiles in the texture
		int32_t num_rows_ { 1 };

		// The total numbers of tiles in the texture (not all rows/cols may be full)
		int32_t num_tiles_ { 1 };

	public:

		ose::unowned_ptr<resources::Texture const> GetTexture() const { return texture_; }
		ose::unowned_ptr<resources::Tilemap const> GetTilemap() const { return tilemap_; }
		int32_t GetNumCols() const { return num_cols_; }
		int32_t GetNumRows() const { return num_rows_; }
		int32_t GetNumTiles() const { return num_tiles_; }

		void SetTexture(ose::unowned_ptr<resources::Texture const> texture) { texture_ = texture; }
		void SetTilemap(ose::unowned_ptr<resources::Tilemap const> tilemap) { tilemap_ = tilemap; }
		void SetNumCols(int32_t n) { if(n > 0) num_cols_ = n; }
		void SetNumRows(int32_t n) { if(n > 0) num_rows_ = n; }
		void SetNumTiles(int32_t n) { if(n > 0) num_tiles_ = n; }

		// Initialise the tile renderer
		TileRenderer(std::string const & name, ose::unowned_ptr<resources::Texture const> t, ose::unowned_ptr<resources::Tilemap const> tm,
			int32_t num_cols, int32_t num_rows, int32_t num_tiles) : Component(name), texture_(t), tilemap_(tm)
		{
			SetNumCols(num_cols);
			SetNumRows(num_rows);
			SetNumTiles(num_tiles);
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
