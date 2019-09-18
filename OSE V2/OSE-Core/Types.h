#pragma once

namespace ose
{
	// alias of raw pointer to relay that pointer does not have ownership
	template <typename T> using unowned_ptr = T*;
}
