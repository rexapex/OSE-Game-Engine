#pragma once
#include "EInputType.h"

namespace ose::input
{
	struct BooleanInput
	{
		EInputType primary_ { EInputType::NONE };
		EInputType secondary_ { EInputType::NONE };
		bool triggered_ { false };
		bool triggered_last_update_ { false };
	};
}
