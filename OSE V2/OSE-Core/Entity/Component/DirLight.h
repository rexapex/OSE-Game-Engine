#pragma once
#include "OSE-Core/Types.h"
#include "Component.h"

namespace ose
{
	// Renders a virtual 3D direction light
	class DirLight : public Component
	{
		// Declare DirLight as an OSE Component
		COMPONENT(DirLight, Component)

	private:

		// Lighting data
		glm::vec3 color_;

	public:

		// Get the color of the light
		glm::vec3 const & GetColor() const { return color_; }
		
		// Initialise the point light
		DirLight(std::string const & name, glm::vec3 const & color) : Component(name), color_(color) {}

		// Does nothing
		virtual ~DirLight() noexcept {}

		// Copy/move constructors
		DirLight(DirLight const & other) noexcept : Component(other), color_(other.color_) {}
		DirLight(DirLight && other) noexcept = default;

		DirLight & operator=(DirLight &) noexcept = delete;
		DirLight & operator=(DirLight &&) noexcept = delete;
	};
}
