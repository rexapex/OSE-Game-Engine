#include "pch.h"
#include "TilemapLoaderFactoryTXT.h"
#include "TilemapLoaderTXT.h"

namespace ose::resources
{
	std::unique_ptr<TilemapLoader> TilemapLoaderFactoryTXT::NewTilemapLoader(std::string const & project_path)
	{
		return std::make_unique<TilemapLoaderTXT>(project_path);
	}
}
