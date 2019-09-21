#pragma once

#include "Component.h"
#include "OSE-Core/Resources/Texture.h"

namespace ose::entity
{
	using namespace resources;

	class TextureFilter : public Component
	{
	// declare TextureFilter as an OSE Component
	COMPONENT(TextureFilter, Component)

	private:
		// NOTE !IMPORTANT! - can't take a pointer to this, it may be re-allocated by ResourceManager
		Texture * texture_;	//pointer should not be de-allocated or re-allocated, resource belongs to ResourceManager

	public:
		//getting and setting texture pointer
		const Texture * const GetTexture() const { return this->texture_; }
		void SetTexture(Texture * texture) { this->texture_ = texture; }

		//allocate rendering engine data
		TextureFilter(const std::string & name) : Component(name) {}

		//de-allocate rendering engine data
		virtual ~TextureFilter() {}

		//copy constructor
		TextureFilter(const TextureFilter & other) noexcept : Component(other), texture_(other.texture_) {}

		//copy assignment constructor
		TextureFilter & operator=(const TextureFilter & other) noexcept
		{
			Component::operator=(other);
			texture_ = other.texture_;
			return *this;
		}

		//move constructor
		TextureFilter(const TextureFilter && other) noexcept : Component(std::move(other)), texture_(other.texture_) {}

		//move assignment constructor
		TextureFilter & operator=(const TextureFilter && other) noexcept
		{
			Component::operator=(std::move(other));
			texture_ = other.texture_;
			return *this;
		}
	};
}
