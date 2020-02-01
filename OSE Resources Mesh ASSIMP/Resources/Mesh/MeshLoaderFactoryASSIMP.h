#pragma once
#include "OSE-Core/Resources/Mesh/MeshLoaderFactory.h"

namespace ose
{
	class MeshLoader;
}

namespace ose::resources
{
	class MeshLoaderFactoryASSIMP final : public MeshLoaderFactory
	{
	public:
		constexpr MeshLoaderFactoryASSIMP() {}
		virtual ~MeshLoaderFactoryASSIMP() {}
		MeshLoaderFactoryASSIMP(MeshLoaderFactoryASSIMP &) = delete;
		MeshLoaderFactoryASSIMP & operator=(MeshLoaderFactoryASSIMP &) = delete;
		MeshLoaderFactoryASSIMP(MeshLoaderFactoryASSIMP &&) = default;
		MeshLoaderFactoryASSIMP & operator=(MeshLoaderFactoryASSIMP &&) = default;

		std::unique_ptr<MeshLoader> NewMeshLoader(std::string const & project_path);
	};
}
