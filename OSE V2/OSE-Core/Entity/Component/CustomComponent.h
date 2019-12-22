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
		std::string component_name_;

	public:

		// Initialise the sprite renderer
		CustomComponent(std::string const & name) : Component(name) {}

		// Does nothing
		virtual ~CustomComponent() noexcept {}

		// Default copy/move constructors
		CustomComponent(CustomComponent const & other) noexcept = default;
		CustomComponent & operator=(CustomComponent const & other) noexcept = default;
		CustomComponent(CustomComponent && other) noexcept = default;
		CustomComponent & operator=(CustomComponent && other) noexcept = default;
	};
}
