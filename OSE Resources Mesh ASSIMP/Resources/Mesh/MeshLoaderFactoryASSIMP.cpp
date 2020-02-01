#include "pch.h"
#include "MeshLoaderFactoryASSIMP.h"
#include "MeshLoaderASSIMP.h"

namespace ose::resources
{
	std::unique_ptr<MeshLoader> MeshLoaderFactoryASSIMP::NewMeshLoader(std::string const & project_path)
	{
		return std::make_unique<MeshLoaderASSIMP>(project_path);
	}
}
