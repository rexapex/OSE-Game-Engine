#pragma once
#include "OSE-Core/Types.h"
#include "Component.h"
#include "OSE-Core/Resources/Texture.h"

namespace ose::entity
{
	using namespace resources;

	// Renders a 2D texture at the entities transform
	class SpriteRenderer : public Component
	{
	// Declare SpriteRenderer as an OSE Component
	COMPONENT(SpriteRenderer, Component)

	private:

		// a sprite renderer is composed of a 2D texture
		// NOTE - this pointer is owned and managed by the resource manager
		ose::unowned_ptr<Texture const> texture_;

	public:

		// set the texture displayed by the sprite renderer
		void SetTexture(ose::unowned_ptr<Texture const> texture) { texture_ = texture; }

		// get the texture displayed by the sprite renderer
		ose::unowned_ptr<Texture const> GetTexture() const { return texture_; }



		// initialise the sprite renderer
		SpriteRenderer(std::string const & name, ose::unowned_ptr<Texture const> t);

		// delete the render_object_ pointer
		virtual ~SpriteRenderer() noexcept;



		// copy constructor
		SpriteRenderer(const SpriteRenderer & other) noexcept;

		// copy assignment constructor
		SpriteRenderer & operator=(const SpriteRenderer & other) noexcept;

		// move constructor
		SpriteRenderer(const SpriteRenderer && other) noexcept;

		// move assignment constructor
		SpriteRenderer & operator=(const SpriteRenderer && other) noexcept;
	};
}
