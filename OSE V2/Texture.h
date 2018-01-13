#pragma once

#include "stdafx.h"
#include "ETextureFilterMode.h"

namespace origami_sheep_engine
{
	//for convenience
	typedef unsigned char * IMGDATA;

	class Texture
	{
	public:
		Texture(const std::string & name, const std::string & path);
		~Texture();
		//copying is not allowed (same as ResourceManager)
		Texture(Texture &) = delete;
		Texture & operator=(Texture &) = delete;
		//moving is allowed (same as ResourceManager)
		Texture(Texture &&) noexcept = default;
		Texture & operator=(Texture &&) noexcept = default;

		const std::string & get_name() const { return name_; }
		const std::string & get_path() const { return path_; }

		void set_name(const std::string & name) { name_ = name; }
		void set_path(const std::string & path) { path_ = path; }

		const int32_t get_width() const { return width_; }
		const int32_t get_height() const { return height_; }
		IMGDATA const get_img_data() const { return img_data_; }

		//set the img data, width and height after the texture has been loaded
		void set_img_data(IMGDATA const img_data, int32_t width, int32_t height) { img_data_ = img_data, width_ = width, height_ = height; }

		const ETextureFilterMode get_mag_filter_mode() const { return mag_filter_mode_; }
		const ETextureFilterMode get_min_filter_mode() const { return min_filter_mode_; }

		void set_mag_filter_mode(const ETextureFilterMode & mode) { mag_filter_mode_ = mode; }
		void set_min_filter_mode(const ETextureFilterMode & mode) { min_filter_mode_ = mode; }

		const bool is_mip_mapping_enabled() const { return mip_mapping_enabled_; }
		const uint32_t get_min_LOD() const { return min_LOD_; }
		const uint32_t get_max_LOD() const { return max_LOD_; }
		const uint32_t get_LOD_bias() const { return LOD_bias_; }

		void set_mip_mapping_enabled(const bool enabled) { mip_mapping_enabled_ = enabled; }
		void set_min_LOD(const uint32_t LOD) { min_LOD_ = LOD; }
		void set_max_LOD(const uint32_t LOD) { max_LOD_ = LOD; }
		void set_LOD_bias(const uint32_t bias) { LOD_bias_ = bias; }

	private:
		std::string name_;
		std::string path_;

		IMGDATA img_data_ { nullptr };
		int32_t width_ { 0 }, height_ { 0 };

		//magnification & minification texture filtering modes
		ETextureFilterMode mag_filter_mode_ { ETextureFilterMode::LINEAR };
		ETextureFilterMode min_filter_mode_ { ETextureFilterMode::LINEAR };

		//mip mapping settings
		bool mip_mapping_enabled_ { true };
		//look this up here: https://www.khronos.org/opengl/wiki/Sampler_Object
		uint32_t min_LOD_;
		uint32_t max_LOD_;
		uint32_t LOD_bias_;
	};
}
