#pragma once
#include "OSE-Core/Resources/Mesh/MeshLoader.h"

namespace ose::resources
{
	class MeshLoaderASSIMP final : public MeshLoader
	{
	public:
		MeshLoaderASSIMP(const std::string & project_path);
		~MeshLoaderASSIMP();
		// MeshLoaderASSIMP is NOT copyable
		MeshLoaderASSIMP(MeshLoaderASSIMP &) = delete;
		MeshLoaderASSIMP & operator=(MeshLoaderASSIMP &) = delete;
		// MeshLoaderASSIMP IS movable
		MeshLoaderASSIMP(MeshLoaderASSIMP &&) noexcept = default;
		MeshLoaderASSIMP & operator=(MeshLoaderASSIMP &&) noexcept = default;

		// Loads the mesh and sets the data in mesh*
		// Path is absolute and is guaranteed to exist
		void LoadMesh(std::string const & path, Mesh * mesh);

		// Free the resources used by the mesh object
		void FreeMesh(Mesh * mesh);

	private:
		std::string project_path_;

		// Process an ASSIMP scene
		void ProcessScene(aiScene const * scene, Mesh * mesh);
	};
}
