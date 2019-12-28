#pragma once
#include "EInputType.h"

namespace ose
{
	struct BooleanInput
	{
		EInputType primary_;
		EInputType secondary_;
		bool triggered_;
		bool triggered_last_update_;
	};
}
