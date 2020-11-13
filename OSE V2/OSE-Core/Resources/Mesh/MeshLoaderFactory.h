#pragma once

namespace ose
{
	class MeshLoader;

	class MeshLoaderFactory
	{
	public:
		constexpr MeshLoaderFactory() {}
		virtual ~MeshLoaderFactory() {}
		MeshLoaderFactory(MeshLoaderFactory &) = delete;
		MeshLoaderFactory & operator=(MeshLoaderFactory &) = delete;
		MeshLoaderFactory(MeshLoaderFactory &&) = default;
		MeshLoaderFactory & operator=(MeshLoaderFactory &&) = default;

		virtual uptr<MeshLoader> NewMeshLoader(std::string const & project_path) = 0;
	};
}
