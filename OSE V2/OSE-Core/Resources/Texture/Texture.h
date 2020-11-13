#pragma once

#include "OSE-Core/Rendering/ETextureFilterMode.h"
#include "TextureMetaData.h"

namespace ose
{
	// for convenience
	typedef unsigned char * IMGDATA;

	class Texture
	{
	protected:
		// Texture is an abstract class, the full class will contain render library specific data
		Texture(std::string const & name, std::string const & path);
	public:
		virtual ~Texture();
		//copying is not allowed (same as ResourceManager)
		Texture(Texture &) = delete;
		Texture & operator=(Texture &) = delete;
		//moving is allowed (same as ResourceManager)
		Texture(Texture &&) noexcept = default;
		Texture & operator=(Texture &&) noexcept = default;

		// the texture can be created in the GPU memory
		virtual void CreateTexture() = 0;

		// the texture can be freed from the GPU memory
		virtual void DestroyTexture() = 0;

		std::string const & GetName() const { return name_; }
		std::string const & GetPath() const { return path_; }

		void SetName(std::string const & name) { name_ = name; }
		void SetPath(std::string const & path) { path_ = path; }

		int32_t GetWidth() const { return width_; }
		int32_t GetHeight() const { return height_; }
		IMGDATA const GetImgData() const { return img_data_; }
		int32_t const GetNumChannels() const { return channels_; }

		//set the img data, width and height after the texture has been loaded
		void SetImgData(IMGDATA const img_data, int32_t width, int32_t height, int32_t channels) { img_data_ = img_data, width_ = width, height_ = height; channels_ = channels; }

		ETextureFilterMode GetMagFilterMode() const { return meta_data_.mag_filter_mode_; }
		ETextureFilterMode GetMinFilterMode() const { return meta_data_.min_filter_mode_; }

		void SetMagFilterMode(ETextureFilterMode mode) { meta_data_.mag_filter_mode_ = mode; }
		void SetMinFilterMode(ETextureFilterMode mode) { meta_data_.min_filter_mode_ = mode; }

		bool IsMipMappingEnabled() const { return meta_data_.mip_mapping_enabled_; }
		uint32_t GetMinLod() const { return meta_data_.min_lod_; }
		uint32_t GetMaxLod() const { return meta_data_.max_lod_; }
		uint32_t GetLodBias() const { return meta_data_.lod_bias_; }

		void SetMipMappingEnabled(bool enabled) { meta_data_.mip_mapping_enabled_ = enabled; }
		void SetMinLod(uint32_t LOD) { meta_data_.min_lod_ = LOD; }
		void SetMaxLod(uint32_t LOD) { meta_data_.max_lod_ = LOD; }
		void SetLodBias(uint32_t bias) { meta_data_.lod_bias_ = bias; }

		//set all meta data in one go
		void SetMetaData(TextureMetaData const & meta_data) { meta_data_ = meta_data; }

	protected:
		std::string name_;
		std::string path_;

		IMGDATA img_data_ { nullptr };
		int32_t width_ { 0 }, height_ { 0 };
		int32_t channels_ { 0 };

		//meta data stored in a separate object for easier (and probably quicker) loading
		TextureMetaData meta_data_;
	};
}
