#include "stdafx.h"
#include "Component.h"

using namespace ose::entity;

const std::size_t Component::Type = std::hash<std::string>()(TO_STRING(Component));

COMPONENT_DEFINITION(Component, TextureFilter)
COMPONENT_DEFINITION(Component, MeshFilter)
COMPONENT_DEFINITION(Component, MeshRenderer)
COMPONENT_DEFINITION(Component, Material)
COMPONENT_DEFINITION(Component, SpriteRenderer)

namespace ose::entity
{
	Component::Component(const std::string & name) : name_(name) {}

	Component::~Component() {}

	Component::Component(const Component & other) noexcept : name_(other.name_) {}

	Component & Component::operator=(const Component & other) noexcept
	{
		name_ = other.name_;
		return *this;
	}

	Component::Component(const Component && other) noexcept : name_(std::move(other.name_)) {}

	Component & Component::operator=(const Component && other) noexcept
	{
		name_ = std::move(other.name_);
		return *this;
	}

	// clone method which can be overwritten by base classes
	std::unique_ptr<Component> Component::clone() const
	{
		return std::make_unique<Component>(*this);
	}

	// disable the component (i.e. remove it from corresponding engine data pool
	void Component::disable()
	{

	}

	// enable the component (i.e. add it to its corresponding engine data pool)
	void Component::enable()
	{

	}
}
