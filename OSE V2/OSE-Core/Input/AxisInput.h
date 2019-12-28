#pragma once
#include "EInputType.h"

namespace ose
{
	struct AxisInput
	{
		EInputType pos_primary_;
		EInputType pos_secondary_;
		EInputType neg_primary_;
		EInputType neg_secondary_;
		double value_;	// Constrained to range [-1, 1]
	};
}
