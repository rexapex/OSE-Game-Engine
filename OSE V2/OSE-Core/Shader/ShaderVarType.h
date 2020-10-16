#pragma once

namespace ose
{
	// Base variable types
	enum class EBaseVarType
	{
		BOOL,
		INT,
		UINT,
		FLOAT,
		DOUBLE
	};

	// Variable consists of a base type alongs with number of row/cols (for vectors/matrices)
	// Variable can be constant across all frag shaders or vary on a per fragment basis
	struct ShaderVarType
	{
		EBaseVarType base_type_;
		uint32_t num_cols_{1};
		uint32_t num_rows_{1};
	};
}
