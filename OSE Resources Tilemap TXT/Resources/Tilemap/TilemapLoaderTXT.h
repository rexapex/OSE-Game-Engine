#pragma once
#include "OSE-Core/Resources/Tilemap/TilemapLoader.h"

namespace ose::resources
{
	class TilemapLoaderTXT : public TilemapLoader
	{
	public:
		TilemapLoaderTXT(std::string const & project_path);
		~TilemapLoaderTXT();
		//TilemapLoaderTXT is NOT copyable
		TilemapLoaderTXT(TilemapLoaderTXT &) = delete;
		TilemapLoaderTXT & operator=(TilemapLoaderTXT &) = delete;
		//TilemapLoaderTXT IS movable
		TilemapLoaderTXT(TilemapLoaderTXT &&) noexcept = default;
		TilemapLoaderTXT & operator=(TilemapLoaderTXT &&) noexcept = default;

		// Loads the tilemap and sets the internal data of the tilemap object
		// Path is absolute and is guaranteed to exist
		virtual void LoadTilemap(std::string const & path, Tilemap & tilemap);
	};
}

