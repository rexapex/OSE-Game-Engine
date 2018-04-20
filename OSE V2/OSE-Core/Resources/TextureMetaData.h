#pragma once

namespace ose::resources
{
	struct TextureMetaData
	{
		//magnification & minification texture filtering modes
		ETextureFilterMode mag_filter_mode_ { ETextureFilterMode::LINEAR };
		ETextureFilterMode min_filter_mode_ { ETextureFilterMode::LINEAR };

		//mip mapping settings
		bool mip_mapping_enabled_ { true };
		//look this up here: https://www.khronos.org/opengl/wiki/Sampler_Object
		uint32_t min_LOD_ { 0 };
		uint32_t max_LOD_ { 0 };
		uint32_t LOD_bias_ { 0 };
	};
}
