#include "stdafx.h"
#include "EditorCamera.h"
#include "OSE-Core/Game/Game.h"

namespace ose
{
	EditorCamera::EditorCamera() : Camera(), Transformable()
	{
	
	}

	EditorCamera::~EditorCamera()
	{
	
	}

	void EditorCamera::Update()
	{
		float delta { static_cast<float>(game_->GetTime().GetDeltaTime()) };

		bool middle_mouse { game_->IsBooleanInputTriggered("MIDDLE_MOUSE") };
		bool lshift { game_->IsBooleanInputTriggered("LSHIFT") };

		if(middle_mouse)
		{
			if(lshift)
			{
				Translate(
					glm::vec3{ game_->GetMouseDx() * delta, 0, 0 } * global_transform_.GetRight()
					+ glm::vec3{ 0, -game_->GetMouseDy() * delta, 0 } * global_transform_.GetUp()
				);
			}
			else
			{
				Rotate(
					glm::vec3{ -game_->GetMouseDy() * delta, 0, 0 } * global_transform_.GetRight()
					+ glm::vec3{ 0, game_->GetMouseDx() * delta, 0 } * global_transform_.GetUp()
				);
			}
		}
	}
}
