#pragma once

#define GLM_FORCE_RADIANS
#include "OSE-Core/EngineDependencies/glm/glm.hpp"
#include "OSE-Core/EngineDependencies/glm/gtc/quaternion.hpp"
#include "OSE-Core/EngineDependencies/glm/gtx/quaternion.hpp"
#include "OSE-Core/EngineDependencies/glm/gtc/type_ptr.hpp"
#include "OSE-Core/EngineDependencies/glm/gtc/matrix_transform.hpp"

namespace ose::math
{
	template <typename T>
	class Transformable;

	class Transform
	{
	template<typename T>
	friend class Transformable;

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

		const glm::vec3 & GetPosition() const { return position_; };
		const glm::quat & GetOrientation() const { return orientation_; };
		const glm::vec3 & GetScale() const { return scale_; };

		const glm::mat4 GetPositionMatrix() const;
		const glm::mat4 GetOrientationMatrix() const;
		const glm::mat4 GetScaleMatrix() const;

		const glm::mat4 GetTransformMatrix() const;

		const glm::vec3 GetUp() const;
		const glm::vec3 GetForward() const;
		const glm::vec3 GetRight() const;

	private:
		void Translate(const glm::vec3 & translation);
		void Translate(const float x, const float y, const float z);

		//rotate by radians
		void Rotate(const glm::vec3 & change);
		void Rotate(const float pitch, const float yaw, const float roll);
		//rotate by degrees
		void RotateDeg(const glm::vec3 & change);
		void RotateDeg(const float pitch, const float yaw, const float roll);

		void Scale(const float scalar);
		void Scale(const glm::vec3 & multiplier);
		void Scale(const float x, const float y, const float z);

		glm::vec3 position_;
		glm::quat orientation_;
		glm::vec3 scale_;
	};
}

