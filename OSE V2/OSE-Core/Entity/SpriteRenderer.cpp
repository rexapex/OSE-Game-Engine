#include "stdafx.h"
#include "SpriteRenderer.h"

namespace ose::entity
{
	SpriteRenderer::SpriteRenderer(std::string const & name, ose::unowned_ptr<Texture const> t) : Component(name), texture_(t)
	{

	}

	SpriteRenderer::~SpriteRenderer() noexcept
	{

	}

	// copy constructor
	SpriteRenderer::SpriteRenderer(const SpriteRenderer & other) noexcept : Component(other), texture_(other.texture_)
	{

	}

	// copy assignment constructor
	SpriteRenderer & SpriteRenderer::operator=(const SpriteRenderer & other) noexcept
	{
		Component::operator=(other);
		texture_ = other.texture_;
		return *this;
	}

	// move constructor
	SpriteRenderer::SpriteRenderer(const SpriteRenderer && other) noexcept : Component(other),
		texture_(other.texture_)
	{

	}

	// move assignment constructor
	SpriteRenderer & SpriteRenderer::operator=(const SpriteRenderer && other) noexcept
	{
		Component::operator=(other);
		texture_ = other.texture_;
		return *this;
	}

}
