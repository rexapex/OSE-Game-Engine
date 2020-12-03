#pragma once
#include "Camera.h"
#include "OSE-Core/Entity/Entity.h"

namespace ose
{
	class FollowCamera : public Camera
	{
	public:
		FollowCamera() : Camera() {}
		virtual ~FollowCamera() {}

		// Update the camera
		virtual void Update() {}

		// Get the global camera transform (i.e. the transform to transform world transforms by to transform into camera space)
		virtual ITransform const & GetGlobalTransform() const { return entity_ ? entity_->GetGlobalTransform() : Transform::IDENTITY; }

		// Accessors/mutators for the entity being followed
		Entity * GetEntity() const { return entity_; }
		void SetEntity(Entity * entity) { entity_ = entity; }

	private:
		Entity * entity_ { nullptr };
	};
}
