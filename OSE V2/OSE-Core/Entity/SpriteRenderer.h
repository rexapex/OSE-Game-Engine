#pragma once
#include "OSE-Core/Types.h"
#include "Component.h"
#include "OSE-Core/Resources/Texture/Texture.h"

namespace ose::entity
{
	// Renders a 2D texture at the entities transform
	class SpriteRenderer : public Component
	{
	// Declare SpriteRenderer as an OSE Component
	COMPONENT(SpriteRenderer, Component)

	private:

		// A sprite renderer is composed of a 2D texture
		// NOTE - this pointer is owned and managed by the resource manager
		ose::unowned_ptr<resources::Texture const> texture_;

	public:

		// Set the texture displayed by the sprite renderer
		void SetTexture(ose::unowned_ptr<resources::Texture const> texture) { texture_ = texture; }

		// Get the texture displayed by the sprite renderer
		ose::unowned_ptr<resources::Texture const> GetTexture() const { return texture_; }


		// Initialise the sprite renderer
		SpriteRenderer(std::string const & name, ose::unowned_ptr<resources::Texture const> t) : Component(name), texture_(t) {}

		// Does nothing
		virtual ~SpriteRenderer() noexcept {}

		// Default copy/move constructors
		SpriteRenderer(SpriteRenderer const & other) noexcept = default;
		SpriteRenderer & operator=(SpriteRenderer const & other) noexcept = default;
		SpriteRenderer(SpriteRenderer && other) noexcept = default;
		SpriteRenderer & operator=(SpriteRenderer && other) noexcept = default;
	};
}
