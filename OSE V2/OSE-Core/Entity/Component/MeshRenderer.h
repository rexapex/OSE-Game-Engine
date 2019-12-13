#pragma once
#include "Component.h"

namespace ose::entity
{
	class MeshRenderer : public Component
	{
	// declare MeshRenderer as an OSE Component
	COMPONENT(MeshRenderer, Component)

	public:

		//data
		//shader reference ?
		//pointer to material data ?



		//allocate rendering engine data
		MeshRenderer(const std::string & name) : Component(name) {}

		//de-allocate rendering engine data
		virtual ~MeshRenderer() {}

		// Default copy/move constructors
		MeshRenderer(MeshRenderer const &) noexcept = default;
		MeshRenderer & operator=(MeshRenderer const &) noexcept = default;
		MeshRenderer(MeshRenderer &&) noexcept = default;
		MeshRenderer & operator=(MeshRenderer &&) noexcept = default;
	};
}
