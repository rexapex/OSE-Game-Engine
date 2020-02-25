#pragma once
#include "OSE-Core/Types.h"
#include "Component.h"
#include "OSE-Core/Resources/Mesh/Mesh.h"

namespace ose
{
	// Renders a 3D mesh at the entities transform
	class MeshRenderer : public Component
	{
		// Declare MeshRenderer as an OSE Component
		COMPONENT(MeshRenderer, Component)

	private:

		// A mesh renderer is composed of a 3D mesh
		// NOTE - this pointer is owned and managed by the resource manager
		unowned_ptr<Mesh const> mesh_;

	public:

		// Set the mesh displayed by the mesh renderer
		void SetMesh(unowned_ptr<Mesh const> mesh) { mesh_ = mesh; }

		// Get the mesh displayed by the mesh renderer
		unowned_ptr<Mesh const> GetMesh() const { return mesh_; }

		// Initialise the mesh renderer
		MeshRenderer(std::string const & name, unowned_ptr<Mesh const> m) : Component(name), mesh_(m) {}

		// Does nothing
		virtual ~MeshRenderer() noexcept {}

		// Default copy/move constructors
		MeshRenderer(MeshRenderer const & other) noexcept = default;
		MeshRenderer(MeshRenderer && other) noexcept = default;

		MeshRenderer & operator=(MeshRenderer &) noexcept = delete;
		MeshRenderer & operator=(MeshRenderer &&) noexcept = delete;
	};
}
