#pragma once
#include "OSE-Core/Types.h"
#include "Component.h"

namespace ose
{
	// Renders a virtual 3D point light
	class PointLight : public Component
	{
		// Declare PointLight as an OSE Component
		COMPONENT(PointLight, Component)

	private:

		// Lighting data
		glm::vec3 color_;

	public:

		// Get the color of the light
		glm::vec3 const & GetColor() const { return color_; }

		// Initialise the point light
		PointLight(std::string const & name, glm::vec3 const & color) : Component(name), color_(color) {}

		// Does nothing
		virtual ~PointLight() noexcept {}

		// Copy/move constructors
		PointLight(PointLight const & other) noexcept : Component(other), color_(other.color_) {}
		PointLight(PointLight && other) noexcept = default;

		PointLight & operator=(PointLight &) noexcept = delete;
		PointLight & operator=(PointLight &&) noexcept = delete;
	};
}
