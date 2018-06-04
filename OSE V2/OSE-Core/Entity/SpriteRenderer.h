#pragma once
#include "Component.h"
#include "OSE-Core/Resources/Texture.h"
#include "OSE-Core/Rendering/RenderObject.h"

namespace ose::entity
{
	using namespace resources;
	using namespace rendering;

	// renders a 2D texture at the entities transform
	class SpriteRenderer : public Component
	{
		// declare SpriteRenderer as an OSE Component
		COMPONENT_DECLERATION(SpriteRenderer)

	public:

		// a sprite renderer is composed of a 2D texture
		// NOTE - this pointer is owned and managed by the resource manager
		const Texture * texture_;

		// provide a pointer to the render data (what is rendererd by OpenGL)
		const RenderObject * render_object_;

		// initialise the sprite renderer
		SpriteRenderer(const std::string & name, const Texture & t) : Component(name), texture_(&t) {}

		// sprite renderer owns/manages no pointers, therefore, nothing to free
		virtual ~SpriteRenderer() {}



		// copy constructor
		SpriteRenderer(const SpriteRenderer & other) noexcept : Component(other), texture_(other.texture_) {}

		// copy assignment constructor
		SpriteRenderer & operator=(const SpriteRenderer & other) noexcept
		{
			Component::operator=(other);
			texture_ = other.texture_;
			return *this;
		}

		// move constructor
		SpriteRenderer(const SpriteRenderer && other) noexcept : Component(other), texture_(other.texture_) {}

		// move assignment constructor
		SpriteRenderer & operator=(const SpriteRenderer && other) noexcept
		{
			Component::operator=(other);
			texture_ = other.texture_;
			return *this;
		}
	};
}
