#pragma once

namespace ose
{
	class Mesh;

	class MeshLoader
	{
	public:
		MeshLoader(const std::string & project_path);
		virtual ~MeshLoader();
		//ModelLoader is NOT copyable
		MeshLoader(MeshLoader &) = delete;
		MeshLoader & operator=(MeshLoader &) = delete;
		//MeshLoader IS movable
		MeshLoader(MeshLoader &&) noexcept = default;
		MeshLoader & operator=(MeshLoader &&) noexcept = default;

		// Loads the mesh and sets the data in mesh*
		// Path is absolute and is guaranteed to exist
		virtual void LoadMesh(std::string const & path, Mesh * mesh) = 0;

		// Free the resources used by the mesh object
		virtual void FreeMesh(Mesh * mesh) = 0;

	private:
		std::string project_path_;
	};
}
