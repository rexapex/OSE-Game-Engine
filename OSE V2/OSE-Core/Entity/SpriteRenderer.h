#pragma once
#include "Component.h"
#include "OSE-Core/Resources/Texture.h"
#include "OSE-Core/Rendering/RenderTask.h"

namespace ose::entity
{
	using namespace resources;
	using namespace rendering;

	// renders a 2D texture at the entities transform
	class SpriteRenderer : public Component
	{
		// declare SpriteRenderer as an OSE Component
		COMPONENT_DECLERATION(SpriteRenderer)

	private:

		// a sprite renderer is composed of a 2D texture
		// NOTE - this pointer is owned and managed by the resource manager
		const Texture * texture_;

		// provide a pointer to the render data (what is rendererd by OpenGL)
		RenderTask * render_object_;

	public:

		// initialise the component, should only be called from the main thread
		// IMPORTANT - can only be called from the thread containing the render context
		virtual void Init() override;


		// get the render object pointer
		RenderTask * GetRenderObject() const { return render_object_; }

		// set the texture displayed by the sprite renderer
		void SetTexture(const Texture * texture) { texture_ = texture; }

		// get the texture displayed by the sprite renderer
		const Texture * GetTexture() const { return texture_; }



		// initialise the sprite renderer
		SpriteRenderer(const std::string & name, const Texture & t);

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
