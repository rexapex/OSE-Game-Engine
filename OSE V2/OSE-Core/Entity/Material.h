#pragma once
#include "Component.h"

namespace ose::entity
{
	class Material : public Component
	{
		// declare MeshFilter as an OSE Component
		COMPONENT_DECLERATION(Material)

	public:

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
	};
}
