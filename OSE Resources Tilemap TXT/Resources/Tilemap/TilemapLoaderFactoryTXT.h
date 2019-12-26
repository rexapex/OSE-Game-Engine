#pragma once
#include "OSE-Core/Resources/Tilemap/TilemapLoaderFactory.h"

namespace ose::resources
{
	class TilemapLoader;

	class TilemapLoaderFactoryTXT : public TilemapLoaderFactory
	{
	public:
		constexpr TilemapLoaderFactoryTXT() {}
		virtual ~TilemapLoaderFactoryTXT() {}
		TilemapLoaderFactoryTXT(TilemapLoaderFactoryTXT &) = delete;
		TilemapLoaderFactoryTXT & operator=(TilemapLoaderFactoryTXT &) = delete;
		TilemapLoaderFactoryTXT(TilemapLoaderFactoryTXT &&) = default;
		TilemapLoaderFactoryTXT & operator=(TilemapLoaderFactoryTXT &&) = default;

		virtual std::unique_ptr<TilemapLoader> NewTilemapLoader(std::string const & project_path);
	};
}
