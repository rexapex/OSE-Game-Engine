#include "stdafx.h"
#include "EditorCamera.h"
#include "OSE-Core/Game/Game.h"

namespace ose
{
	void EditorCamera::Update()
	{
		float delta { static_cast<float>(game_->GetTime().GetDeltaTime()) };
		float move_x { static_cast<float>(game_->GetAxisValue("move_x")) };
		float move_y { static_cast<float>(game_->GetAxisValue("move_y")) };
		float speed { game_->IsBooleanInputTriggered("speed") ? 5.0f : 1.0f };
		Translate(move_x * delta * speed, 0, move_y * delta * speed);
	}
}
