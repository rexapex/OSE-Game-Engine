#pragma once
#include "Component.h"

namespace ose::entity
{
	class MeshRenderer : public Component
	{
		// declare MeshRenderer as an OSE Component
		COMPONENT_DECLERATION(MeshRenderer)

	public:

		//data
		//shader reference ?
		//pointer to material data ?



		//allocate rendering engine data
		MeshRenderer(const std::string & name) : Component(name) {}

		//de-allocate rendering engine data
		virtual ~MeshRenderer() {}



		//copy constructor
		MeshRenderer(const MeshRenderer & other) noexcept : Component(other) {}

		//copy assignment constructor
		MeshRenderer & operator=(const MeshRenderer & other) noexcept
		{
			Component::operator=(other);
			return *this;
		}

		//move constructor
		MeshRenderer(const MeshRenderer && other) noexcept : Component(other) {}

		//move assignment constructor
		MeshRenderer & operator=(const MeshRenderer && other) noexcept
		{
			Component::operator=(other);
			return *this;
		}
	};
}