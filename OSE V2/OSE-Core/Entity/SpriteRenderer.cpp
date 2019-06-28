#include "stdafx.h"
#include "SpriteRenderer.h"

namespace ose::entity
{
	SpriteRenderer::SpriteRenderer(const std::string & name, const Texture & t) : Component(name), texture_(&t)
	{
		render_object_ = new RenderTaskImpl();
	}

	// delete the render_object_ pointer
	SpriteRenderer::~SpriteRenderer() noexcept
	{
		if(render_object_)
		{
			delete render_object_;
		}
	}

	// copy constructor
	SpriteRenderer::SpriteRenderer(const SpriteRenderer & other) noexcept : Component(other), texture_(other.texture_)
	{
		// copy the render object
		render_object_ = new RenderTaskImpl(*static_cast<RenderTaskImpl*>(other.render_object_));
	}

	// copy assignment constructor
	SpriteRenderer & SpriteRenderer::operator=(const SpriteRenderer & other) noexcept
	{
		Component::operator=(other);
		texture_ = other.texture_;
		render_object_ = other.render_object_;
		// copy the render object
		render_object_ = new RenderTaskImpl(*static_cast<RenderTaskImpl*>(other.render_object_));
		return *this;
	}

	// move constructor
	SpriteRenderer::SpriteRenderer(const SpriteRenderer && other) noexcept : Component(other),
		texture_(other.texture_), render_object_(other.render_object_)
	{
		// move the render object
		render_object_ = std::move(static_cast<RenderTaskImpl*>(other.render_object_));
	}

	// move assignment constructor
	SpriteRenderer & SpriteRenderer::operator=(const SpriteRenderer && other) noexcept
	{
		Component::operator=(other);
		texture_ = other.texture_;
		render_object_ = other.render_object_;
		// move the render object
		render_object_ = std::move(static_cast<RenderTaskImpl*>(other.render_object_));
		return *this;
	}

	// initialise the component, should only be called from the main thread
	// IMPORTANT - can only be called from the thread containing the render context
	void SpriteRenderer::init()
	{
		// initialise the render object
		// NOTE - requires render context
		render_object_->init(this);
	}
}
