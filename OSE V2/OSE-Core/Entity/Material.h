#pragma once
#include "Component.h"

namespace ose::entity
{
	class Material : public Component
	{
	// declare MeshFilter as an OSE Component
	COMPONENT(Material, Component)

	public:

		//allocate rendering engine data
		Material(const std::string & name) : Component(name) {}

		//de-allocate rendering engine data
		virtual ~Material() {}

		// Default copy/move constructors
		Material(Material const &) noexcept = default;
		Material & operator=(Material const &) noexcept = default;
		Material(Material &&) noexcept = default;
		Material & operator=(Material &&) noexcept = default;
	};
}
