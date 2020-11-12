#include "stdafx.h"
#include "Component.h"

namespace ose
{
	Component::Component(std::string const & name) : name_(name), enabled_(true) {}

	Component::~Component() {}

	Component::Component(Component const & other) noexcept : name_(other.name_), enabled_(other.enabled_) {}
	
	// clone method which can be overwritten by base classes
	uptr<Component> Component::Clone() const
	{
		return nullptr;
		///return ose::make_unique<Component>(*this);
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
