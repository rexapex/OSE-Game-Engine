#include "pch.h"
#include "MeshLoaderFactoryASSIMP.h"
#include "MeshLoaderASSIMP.h"

namespace ose::resources
{
	uptr<MeshLoader> MeshLoaderFactoryASSIMP::NewMeshLoader(std::string const & project_path)
	{
		return ose::make_unique<MeshLoaderASSIMP>(project_path);
	}
}
