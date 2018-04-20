#pragma once
#include "../../stdafx.h"

namespace ose::entity
{
	struct Component
	{
		//fields shared by all component types
		std::string name;



		//constructor
		Component(const std::string & n) : name(n) {}

		//allow sub classes to implement destructors
		virtual ~Component() {}



		//copy constructor
		Component(const Component & other) noexcept : name(other.name) {}

		//copy assignment constructor
		Component & operator=(const Component & other) noexcept
		{
			name = other.name;
			return *this;
		}

		//move constructor
		Component(const Component && other) noexcept : name(std::move(other.name)) {}

		//move assignment constructor
		Component & operator=(const Component && other) noexcept
		{
			name = std::move(other.name);
			return *this;
		}

		//clone method which can be overwritten by base classes
		virtual std::unique_ptr<Component> clone() const
		{
			return std::make_unique<Component>(*this);
		}
	};
}
