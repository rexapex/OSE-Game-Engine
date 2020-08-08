#pragma once

#include "OSE-Core/Rendering/EProjectionMode.h"

namespace ose
{
	struct RenderingSettings
	{
		// Global settings
		EProjectionMode projection_mode_ { EProjectionMode::ORTHOGRAPHIC };

		// EProjectionMode::PERSPECTIVE settings
		float znear_	{ 0.01f };
		float zfar_		{ 100.0f };
		float hfov_		{ 60.0f };
	};

	struct ProjectSettings
	{
		RenderingSettings rendering_settings_;
	};
}
