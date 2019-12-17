#pragma once
#include "OSE-Core/Types.h"
#include "Component.h"
#include "OSE-Core/Resources/Texture/Texture.h"

namespace ose::entity
{
	// Renders a 2D grid of tile textures at the entities transform
	class TileRenderer : public Component
	{
		// Declare TileRenderer as an OSE Component
		COMPONENT(TileRenderer, Component)
		
	private:
		// The texture to use as the atlas
		// NOTE - this pointer is owned and managed by the resource manager
		ose::unowned_ptr<resources::Texture const> texture_;

		// The number of columns of tiles in the texture
		uint32_t num_cols_;

		// The number of rows of tiles in the texture
		uint32_t num_rows_;

		// The total numbers of tiles in the texture (not all rows/cols may be full)
		uint32_t num_tiles_;

	public:

		ose::unowned_ptr<resources::Texture const> GetTexture() const { return texture_; }
		uint32_t GetNumCols() const { return num_cols_; }
		uint32_t GetNumRows() const { return num_rows_; }
		uint32_t GetNumTiles() const { return num_tiles_; }

		void SetTexture(ose::unowned_ptr<resources::Texture const> texture) { texture_ = texture; }
		void SetNumCols(uint32_t n) { if(n > 0) num_cols_ = n; }
		void SetNumRows(uint32_t n) { if(n > 0) num_rows_ = n; }
		void SetNumTiles(uint32_t n) { if(n > 0) num_tiles_ = n; }

		// Initialise the tile renderer
		TileRenderer(std::string const & name, ose::unowned_ptr<resources::Texture const> t,
			uint32_t num_cols, uint32_t num_rows, uint32_t num_tiles) : Component(name), texture_(t),
			num_cols_(num_cols), num_rows_(num_rows), num_tiles_(num_tiles) {}

		// Does nothing
		virtual ~TileRenderer() noexcept {}

		// Default copy/move constructors
		TileRenderer(TileRenderer const & other) noexcept = default;
		TileRenderer & operator=(TileRenderer const & other) noexcept = default;
		TileRenderer(TileRenderer && other) noexcept = default;
		TileRenderer & operator=(TileRenderer && other) noexcept = default;
	};
}
