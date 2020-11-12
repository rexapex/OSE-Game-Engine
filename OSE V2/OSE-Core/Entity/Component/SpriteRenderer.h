#pragma once
#include "OSE-Core/Types.h"
#include "Component.h"
#include "OSE-Core/Resources/Texture/Texture.h"

namespace ose
{
	// Renders a 2D texture at the entities transform
	class SpriteRenderer : public Component
	{
	// Declare SpriteRenderer as an OSE Component
	COMPONENT(SpriteRenderer, Component)

	private:
	
		// A sprite renderer is composed of a 2D texture
		// NOTE - this pointer is owned and managed by the resource manager
		Texture const * texture_;

	public:

		// Set the texture displayed by the sprite renderer
		void SetTexture(Texture const * texture) { texture_ = texture; }

		// Get the texture displayed by the sprite renderer
		Texture const * GetTexture() const { return texture_; }

		// Initialise the sprite renderer
		SpriteRenderer(std::string const & name, Texture const * t) : Component(name), texture_(t) {}

		// Does nothing
		virtual ~SpriteRenderer() noexcept {}

		// Default copy/move constructors
		SpriteRenderer(SpriteRenderer const & other) noexcept = default;
		SpriteRenderer(SpriteRenderer && other) noexcept = default;

		SpriteRenderer & operator=(SpriteRenderer &) noexcept = delete;
		SpriteRenderer & operator=(SpriteRenderer &&) noexcept = delete;
	};
}
