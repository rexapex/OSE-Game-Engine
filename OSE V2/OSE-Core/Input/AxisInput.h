#pragma once
#include "EInputType.h"

namespace ose::input
{
	struct AxisInput
	{
		EInputType pos_primary_;
		EInputType pos_secondary_;
		EInputType neg_primary_;
		EInputType neg_secondary_;
		double pos_value_;	// Constrained to range [0, 1]
		double neg_value_;	// Constrained to range [0, 1]
		double value_;		// Constrained to range [-1, 1]
	};
}
