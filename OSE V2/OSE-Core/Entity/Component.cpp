#include "stdafx.h"
#include "Component.h"

using namespace ose::entity;

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
	std::unique_ptr<Component> Component::Clone() const
	{
		return nullptr;
		///return std::make_unique<Component>(*this);
	}

	// disable the component (i.e. remove it from corresponding engine data pool
	void Component::Disable()
	{

	}

	// enable the component (i.e. add it to its corresponding engine data pool)
	void Component::Enable()
	{

	}
}
