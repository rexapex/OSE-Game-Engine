#pragma once
#include "EInputType.h"

namespace ose::input
{
	struct AxisInput
	{
		EInputType pos_primary_ { EInputType::NONE };
		EInputType pos_secondary_ { EInputType::NONE };
		EInputType neg_primary_ { EInputType::NONE };
		EInputType neg_secondary_ { EInputType::NONE };
		double pos_value_ { 0.0 };	// Constrained to range [0, 1]
		double neg_value_ { 0.0 };	// Constrained to range [0, 1]
		double value_ { 0.0 };		// Constrained to range [-1, 1]
	};
}
