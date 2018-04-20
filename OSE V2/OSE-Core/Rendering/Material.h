#pragma once
#include "../../stdafx.h"
#include "../Entity/Component.h"

namespace ose::rendering
{
	using namespace entity;
	using namespace rendering;

	struct Material : public Component
	{
		//data



		//allocate rendering engine data
		Material(const std::string & name) : Component(name) {}

		//de-allocate rendering engine data
		virtual ~Material() {}



		//copy constructor
		Material(const Material & other) noexcept : Component(other) {}

		//copy assignment constructor
		Material & operator=(const Material & other) noexcept
		{
			Component::operator=(other);
			return *this;
		}

		//move constructor
		Material(const Material && other) noexcept : Component(other) {}

		//move assignment constructor
		Material & operator=(const Material && other) noexcept
		{
			Component::operator=(other);
			return *this;
		}

		//clone method which can be overwritten by base classes
		virtual std::unique_ptr<Component> clone() const
		{
			return std::make_unique<Material>(*this);
		}
	};
}
