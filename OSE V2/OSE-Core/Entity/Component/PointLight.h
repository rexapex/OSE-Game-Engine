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
		float intensity_;

	public:

		// Get the color of the light
		glm::vec3 const & GetColor() const { return color_; }

		// Get the intensity of the light
		float GetIntensity() const { return intensity_; }

		// Initialise the point light
		PointLight(std::string const & name, glm::vec3 const & color, float intensity) : Component(name), color_(color), intensity_(intensity) {}

		// Does nothing
		virtual ~PointLight() noexcept {}

		// Copy/move constructors
		PointLight(PointLight const & other) noexcept : Component(other), color_(other.color_), intensity_(other.intensity_) {}
		PointLight(PointLight && other) noexcept = default;

		PointLight & operator=(PointLight &) noexcept = delete;
		PointLight & operator=(PointLight &&) noexcept = delete;
	};
}
