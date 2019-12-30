#pragma once
#include "BooleanInput.h"
#include "AxisInput.h"

namespace ose::input
{
	struct InputSettings
	{
		std::unordered_map<std::string, BooleanInput> boolean_inputs_;
		std::unordered_map<std::string, AxisInput> axis_inputs_;
	};
}
