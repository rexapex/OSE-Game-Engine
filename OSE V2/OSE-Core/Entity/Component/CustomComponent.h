#pragma once
#include "Component.h"

namespace ose::entity
{
	class CustomComponent : public Component
	{
		// Declare CustomComponent as an OSE Component
		COMPONENT(CustomComponent, Component)

	private:

		// The name of the custom component
		// Multiple custom components can share the same component_name_
		// The component_name_ determines the script to use
		std::string component_type_name_;

		// The data used to initialise the custom component data structure
		std::initializer_list<std::any> data_;

	public:

		// Get the type name of the custom component
		std::string const & GetComponentTypeName() const { return component_type_name_; }

		// Set the type name of the custom component
		void SetComponentTypeName(std::string const & name) { component_type_name_ = name; }

		// Initialise the sprite renderer
		CustomComponent(std::string const & name, std::string const & type_name) : Component(name), component_type_name_(type_name) {}

		// Does nothing
		virtual ~CustomComponent() noexcept {}

		// Default move constructors
		CustomComponent(CustomComponent && other) noexcept = default;
		CustomComponent & operator=(CustomComponent && other) noexcept = default;

		// Need to manually define copy constructors since std::string in non-copyable
		CustomComponent(CustomComponent const & other) noexcept
			: Component(other), component_type_name_(other.component_type_name_) {}
		CustomComponent & operator=(CustomComponent const & other) noexcept
		{
			Component::operator=(other);
			component_type_name_ = other.component_type_name_;
			return *this;
		}
	};
}
