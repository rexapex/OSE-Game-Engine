#include "stdafx.h"
#include "EditorCamera.h"

namespace ose
{
	void EditorCamera::Update()
	{
		Translate(0.001f, 0, 0);
	}
}
