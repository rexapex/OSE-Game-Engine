#pragma once
#include "OSE-Core/Math/Transform.h"

namespace ose
{
	class Game;

	class Camera
	{
	public:
		Camera() {}
		virtual ~Camera() {}

		// Update the camera
		virtual void Update() {}

		// Should NEVER be called directly by a script
		void SetGameReference(Game * game) { game_ = game; }

		// Get the global camera transform (i.e. the transform to transform world transforms by to transform into camera space)
		virtual ITransform const & GetGlobalTransform() const { return Transform::IDENTITY; }

	protected:
		Game * game_ { nullptr };
	};
}
