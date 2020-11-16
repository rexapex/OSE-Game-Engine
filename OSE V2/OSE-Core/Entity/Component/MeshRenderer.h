#pragma once
#include "OSE-Core/Types.h"
#include "Component.h"
#include "OSE-Core/Resources/Mesh/Mesh.h"
#include "OSE-Core/Resources/Material/Material.h"

namespace ose
{
	// Renders a 3D mesh at the entities transform
	class MeshRenderer : public Component
	{
		// Declare MeshRenderer as an OSE Component
		COMPONENT(MeshRenderer, Component)

	private:

		// A mesh renderer is composed of a 3D mesh and a material
		// NOTE - these pointers is owned and managed by the resource manager
		Mesh const * mesh_			{ nullptr };
		Material const * material_	{ nullptr };

	public:

		// Set the mesh displayed by the mesh renderer
		void SetMesh(Mesh const * mesh) { mesh_ = mesh; }

		// Get the mesh displayed by the mesh renderer
		Mesh const * GetMesh() const { return mesh_; }

		// Set the material used to shade the mesh
		void SetMaterial(Material const * material) { material_ = material; }

		// Get the material used to shade the mesh
		Material const * GetMaterial() const { return material_; }

		// Initialise the mesh renderer
		MeshRenderer(std::string const & name, Mesh const * m, Material const * mat) : Component(name), mesh_(m), material_(mat) {}

		// Does nothing
		virtual ~MeshRenderer() noexcept {}

		// Default copy/move constructors
		MeshRenderer(MeshRenderer const & other) noexcept = default;
		MeshRenderer(MeshRenderer && other) noexcept = default;

		MeshRenderer & operator=(MeshRenderer &) noexcept = delete;
		MeshRenderer & operator=(MeshRenderer &&) noexcept = delete;
	};
}
