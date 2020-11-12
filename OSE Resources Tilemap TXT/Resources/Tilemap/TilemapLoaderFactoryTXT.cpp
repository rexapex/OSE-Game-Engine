#include "pch.h"
#include "TilemapLoaderFactoryTXT.h"
#include "TilemapLoaderTXT.h"

namespace ose::resources
{
	uptr<TilemapLoader> TilemapLoaderFactoryTXT::NewTilemapLoader(std::string const & project_path)
	{
		return ose::make_unique<TilemapLoaderTXT>(project_path);
	}
}
