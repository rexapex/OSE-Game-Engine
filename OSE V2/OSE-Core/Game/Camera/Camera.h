#pragma once

#include "OSE-Core/Math/Transformable.h"

namespace ose
{
	class Game;

	class Camera : public Transformable<uptr<Camera>>
	{
	public:
		Camera() : Transformable() {}
		virtual ~Camera() {}

		// Get a list of transformable elements
		// Returns an empty dummy list
		virtual std::vector<uptr<Camera>> const & GetChildTransformables() const override { return dummy_; }

		// Get a pointer to the parent transformable element
		virtual Transformable * GetParentTransformable() const override { return nullptr; }

		// Update the camera
		virtual void Update() {}

		// Should NEVER be called directly by a script
		void SetGameReference(Game * game) { game_ = game; }

	protected:
		Game * game_ { nullptr };

	private:
		std::vector<uptr<Camera>> dummy_;
	};
}
