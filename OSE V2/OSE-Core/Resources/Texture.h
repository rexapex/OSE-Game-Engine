#pragma once

#include "OSE-Core/Rendering/ETextureFilterMode.h"
#include "TextureMetaData.h"

namespace ose::resources
{
	// for convenience
	typedef unsigned char * IMGDATA;

	class Texture
	{
	protected:
		// Texture is an abstract class, the full class will contain render library specific data
		Texture(const std::string & name, const std::string & path);
	public:
		~Texture();
		//copying is not allowed (same as ResourceManager)
		Texture(Texture &) = delete;
		Texture & operator=(Texture &) = delete;
		//moving is allowed (same as ResourceManager)
		Texture(Texture &&) noexcept = default;
		Texture & operator=(Texture &&) noexcept = default;

		// in order to be useful, the texture must be bindable
		virtual void Bind() = 0;

		// the texture can be created in the GPU memory
		virtual void CreateTexture() = 0;

		// the texture can be freed from the GPU memory
		virtual void DestroyTexture() = 0;

		const std::string & GetName() const { return name_; }
		const std::string & GetPath() const { return path_; }

		void SetName(const std::string & name) { name_ = name; }
		void SetPath(const std::string & path) { path_ = path; }

		const int32_t GetWidth() const { return width_; }
		const int32_t GetHeight() const { return height_; }
		IMGDATA const GetImgData() const { return img_data_; }

		//set the img data, width and height after the texture has been loaded
		void SetImgData(IMGDATA const img_data, int32_t width, int32_t height) { img_data_ = img_data, width_ = width, height_ = height; }

		const ETextureFilterMode GetMagFilterMode() const { return meta_data_.mag_filter_mode_; }
		const ETextureFilterMode GetMinFilterMode() const { return meta_data_.min_filter_mode_; }

		void SetMagFilterMode(const ETextureFilterMode & mode) { meta_data_.mag_filter_mode_ = mode; }
		void SetMinFilterMode(const ETextureFilterMode & mode) { meta_data_.min_filter_mode_ = mode; }

		const bool IsMipMappingEnabled() const { return meta_data_.mip_mapping_enabled_; }
		const uint32_t GetMinLod() const { return meta_data_.min_lod_; }
		const uint32_t GetMaxLod() const { return meta_data_.max_lod_; }
		const uint32_t GetLodBias() const { return meta_data_.lod_bias_; }

		void SetMipMappingEnabled(const bool enabled) { meta_data_.mip_mapping_enabled_ = enabled; }
		void SetMinLod(const uint32_t LOD) { meta_data_.min_lod_ = LOD; }
		void SetMaxLod(const uint32_t LOD) { meta_data_.max_lod_ = LOD; }
		void SetLodBias(const uint32_t bias) { meta_data_.lod_bias_ = bias; }

		//set all meta data in one go
		void SetMetaData(const TextureMetaData & meta_data) { meta_data_ = meta_data; }

	protected:
		std::string name_;
		std::string path_;

		IMGDATA img_data_ { nullptr };
		int32_t width_ { 0 }, height_ { 0 };

		//meta data stored in a separate object for easier (and probably quicker) loading
		TextureMetaData meta_data_;
	};
}
