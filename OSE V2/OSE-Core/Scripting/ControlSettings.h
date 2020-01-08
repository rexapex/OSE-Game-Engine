#pragma once
#include "CustomControl.h"

namespace ose
{
	struct ControlSettings
	{
		std::vector<CustomControl> controls_;
		std::vector<CustomControl> deferred_controls_;
	};
}
