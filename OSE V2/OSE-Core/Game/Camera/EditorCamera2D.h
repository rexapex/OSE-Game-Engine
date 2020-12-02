#pragma once
#include "Camera.h"
#include "OSE-Core/Entity/Entity.h"
#include "OSE-Core/Game/Game.h"

namespace ose
{
	class EditorCamera2D : public Camera
	{
	public:
		EditorCamera2D() : Camera(), entity_(std::make_unique<Entity>(nullptr, "OSE-Camera-Stub")) {}
		virtual ~EditorCamera2D() {}

		// Update the camera
		virtual void Update()
		{
			float delta { static_cast<float>(game_->GetTime().GetDeltaTime()) };
			bool lshift { game_->IsBooleanInputTriggered("OSE-Camera-Move-Fast") };
			float ms { lshift ? 1000.0f : 250.0f };

			float dx { delta * ms * static_cast<float>(game_->GetAxisValue("OSE-Camera-Move-X")) };
			float dy { delta * ms * static_cast<float>(game_->GetAxisValue("OSE-Camera-Move-Y")) };

			entity_->Translate2d(-dx, dy);
		}

		// Get the global camera transform (i.e. the transform to transform world transforms by to transform into camera space)
		virtual ITransform const & GetGlobalTransform() const { return entity_ ? entity_->GetGlobalTransform() : Transform::IDENTITY; }

	private:
		std::unique_ptr<Entity> entity_;
	};
}
