#pragma once
#include "../Resources/Texture.h"
#include "Renderer.h"

namespace ose::rendering
{
	struct SpriteRenderer : public Renderer
	{
		//data
		//texture to render
		Texture * texture;
		//should texture be flipped in x
		bool flip_x;
		//should texture be flipped in y
		bool flip_y;
		//shader reference ?
		//pointer to material data ?



		//allocate rendering engine data
		SpriteRenderer(const std::string & name) : Renderer(name) {}

		//de-allocate rendering engine data
		virtual ~SpriteRenderer() {}



		//copy constructor
		SpriteRenderer(const SpriteRenderer & other) noexcept : Renderer(other) {}

		//copy assignment constructor
		SpriteRenderer & operator=(const SpriteRenderer & other) noexcept
		{
			Renderer::operator=(other);
			return *this;
		}

		//move constructor
		SpriteRenderer(const SpriteRenderer && other) noexcept : SpriteRenderer(other) {}

		//move assignment constructor
		SpriteRenderer & operator=(const SpriteRenderer && other) noexcept
		{
			Renderer::operator=(other);
			return *this;
		}

		//clone method which can be overwritten by base classes
		/*virtual std::unique_ptr<Component> clone() const
		{
			return std::make_unique<SpriteRenderer>(*this);
		}*/
	};
}
