#pragma once

#define GLM_FORCE_RADIANS
#include "OSE-Core/EngineDependencies/glm/glm.hpp"
#include "OSE-Core/EngineDependencies/glm/gtc/quaternion.hpp"
#include "OSE-Core/EngineDependencies/glm/gtx/quaternion.hpp"
#include "OSE-Core/EngineDependencies/glm/gtc/type_ptr.hpp"
#include "OSE-Core/EngineDependencies/glm/gtc/matrix_transform.hpp"

namespace ose::math
{
	class Transform
	{
	public:
		//constructors & destructor
		Transform();
		Transform(const glm::vec3 & position);
		Transform(const glm::vec3 & position, const glm::vec3 & rotation_radians, const glm::vec3 & scale);
		Transform(const glm::vec3 & position, const glm::quat & orientation, const glm::vec3 & scale);
		~Transform();

		//copy constructors
		Transform(const Transform & other) noexcept;
		Transform & operator=(const Transform & other) noexcept;

		//move constructors
		Transform(Transform && other) noexcept;
		Transform & operator=(Transform && other) noexcept;

		void translate(const glm::vec3 & translation);
		void translate(const float x, const float y, const float z);

		//rotate by radians
		void rotate(const glm::vec3 & change);
		void rotate(const float pitch, const float yaw, const float roll);
		//rotate by degrees
		void rotateDeg(const glm::vec3 & change);
		void rotateDeg(const float pitch, const float yaw, const float roll);

		void scale(const float scalar);
		void scale(const glm::vec3 & multiplier);
		void scale(const float x, const float y, const float z);

		const glm::vec3 & get_position() const { return position_; };
		const glm::quat & get_orientation() const { return orientation_; };
		const glm::vec3 & get_scale() const { return scale_; };

		const glm::mat4 get_position_as_matrix() const;
		const glm::mat4 get_orientation_as_matrix() const;
		const glm::mat4 get_scale_as_matrix() const;

		const glm::mat4 get_transform_matrix() const;

		const glm::vec3 get_up() const;
		const glm::vec3 get_forward() const;
		const glm::vec3 get_right() const;

	private:
		glm::vec3 position_;
		glm::quat orientation_;
		glm::vec3 scale_;
	};
}

