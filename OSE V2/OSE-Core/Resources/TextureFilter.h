#pragma once

#include "../../stdafx.h"
#include "ResourceFilter.h"
#include "Texture.h"

namespace ose::resources
{
	struct TextureFilter : public ResourceFilter
	{
	private:
		Texture * texture_;	//pointer should not be de-allocated or re-allocated, resource belongs to ResourceManager

	public:
		//getting and setting texture pointer
		const Texture * const get_texture() const { return this->texture_; }
		void set_texture(Texture * texture) { this->texture_ = texture; }

		//allocate rendering engine data
		TextureFilter(const std::string & name) : ResourceFilter(name) {}

		//de-allocate rendering engine data
		virtual ~TextureFilter() {}

		//copy constructor
		TextureFilter(const TextureFilter & other) noexcept : ResourceFilter(other), texture_(other.texture_) {}

		//copy assignment constructor
		TextureFilter & operator=(const TextureFilter & other) noexcept
		{
			ResourceFilter::operator=(other);
			texture_ = other.texture_;
			return *this;
		}

		//move constructor
		TextureFilter(const TextureFilter && other) noexcept : ResourceFilter(other), texture_(other.texture_) {}

		//move assignment constructor
		TextureFilter & operator=(const TextureFilter && other) noexcept
		{
			TextureFilter::operator=(other);
			texture_ = other.texture_;
			return *this;
		}
	};
}
