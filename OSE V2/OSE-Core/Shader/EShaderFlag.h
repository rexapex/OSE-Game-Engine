#pragma once

namespace ose
{
	enum EShaderFlag
	{
		SF_NONE = 0,
		SF_2D = 1,					// Shaders default to 3D
		SF_SKINNED = 2				// Shaders default to static
	};
}
