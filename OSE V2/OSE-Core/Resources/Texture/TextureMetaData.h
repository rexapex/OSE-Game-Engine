#pragma once

namespace ose
{
	struct TextureMetaData
	{
		//magnification & minification texture filtering modes
		ETextureFilterMode mag_filter_mode_ { ETextureFilterMode::LINEAR };
		ETextureFilterMode min_filter_mode_ { ETextureFilterMode::LINEAR };

		//mip mapping settings
		bool mip_mapping_enabled_ { true };
		//look this up here: https://www.khronos.org/opengl/wiki/Sampler_Object
		uint32_t min_lod_ { 0 };
		uint32_t max_lod_ { 0 };
		uint32_t lod_bias_ { 0 };
	};
}
