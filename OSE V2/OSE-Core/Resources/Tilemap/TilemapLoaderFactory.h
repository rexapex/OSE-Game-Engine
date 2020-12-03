#pragma once

namespace ose
{
	class TilemapLoader;

	class TilemapLoaderFactory
	{
	public:
		constexpr TilemapLoaderFactory() {}
		virtual ~TilemapLoaderFactory() {}
		TilemapLoaderFactory(TilemapLoaderFactory &) = delete;
		TilemapLoaderFactory & operator=(TilemapLoaderFactory &) = delete;
		TilemapLoaderFactory(TilemapLoaderFactory &&) = default;
		TilemapLoaderFactory & operator=(TilemapLoaderFactory &&) = default;

		virtual uptr<TilemapLoader> NewTilemapLoader(std::string const & project_path) = 0;
	};
}
