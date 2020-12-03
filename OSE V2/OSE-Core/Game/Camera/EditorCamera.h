#pragma once
#include "Camera.h"
#include "OSE-Core/Math/Transformable.h"

namespace ose
{
	class EditorCamera : public Camera, public Transformable<uptr<EditorCamera>>
	{
	public:
		EditorCamera();
		virtual ~EditorCamera();

		// Update the camera
		virtual void Update();

		// Get a list of transformable elements
		// Returns an empty dummy list
		virtual std::vector<uptr<EditorCamera>> const & GetChildTransformables() const override { return dummy_; }

		// Get a pointer to the parent transformable element
		virtual Transformable * GetParentTransformable() const override { return nullptr; }

		// Get the global camera transform (i.e. the transform to transform world transforms by to transform into camera space)
		virtual ITransform const & GetGlobalTransform() const { return global_transform_; }

	private:
		std::vector<uptr<EditorCamera>> dummy_;
	};
}
