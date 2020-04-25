#pragma once

#include "OSE-Core/Math/Transformable.h"

namespace ose
{
	class Game;

	class Camera : public Transformable<std::unique_ptr<Camera>>
	{
	public:
		Camera() {}
		virtual ~Camera() {}

		// Get a list of transformable elements
		// Returns an empty dummy list
		const std::vector<std::unique_ptr<Camera>> & GetChildTransformables() override { return dummy_; }

		// Update the camera
		virtual void Update() {}

		// Should NEVER be called directly by a script
		void SetGameReference(unowned_ptr<Game> game) { game_ = game; }

	protected:
		unowned_ptr<Game> game_;

	private:
		std::vector<std::unique_ptr<Camera>> dummy_;
	};
}
