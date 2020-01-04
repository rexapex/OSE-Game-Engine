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

	struct ShaderVarType
	{
		EBaseVarType base_type_;
		uint32_t num_cols_{1};
		uint32_t num_rows_{1};
	};
}
