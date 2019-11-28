#pragma once

#define GLM_FORCE_RADIANS
#include "OSE-Core/EngineDependencies/glm/glm.hpp"
#include "OSE-Core/EngineDependencies/glm/gtc/quaternion.hpp"
#include "OSE-Core/EngineDependencies/glm/gtx/quaternion.hpp"
#include "OSE-Core/EngineDependencies/glm/gtc/type_ptr.hpp"
#include "OSE-Core/EngineDependencies/glm/gtc/matrix_transform.hpp"
#include "ITransform.h"

namespace ose::math
{
	class Transform : public ITransform
	{
	public:
		// Constructors & destructor
		Transform();
		Transform(const glm::vec3 & position);
		Transform(const glm::vec3 & position, const glm::vec3 & rotation_radians, const glm::vec3 & scale);
		Transform(const glm::vec3 & position, const glm::quat & orientation, const glm::vec3 & scale);
		~Transform();

		// Copy from ITransform to Transform object
		Transform(const ITransform & other) noexcept;

		// Copy constructors
		Transform(const Transform & other) noexcept;
		Transform & operator=(const Transform & other) noexcept;

		// Move constructors
		Transform(Transform && other) noexcept;
		Transform & operator=(Transform && other) noexcept;

		const glm::vec3 & GetPosition() const override { return position_; };
		const glm::quat & GetOrientation() const override { return orientation_; };
		const glm::vec3 & GetScale() const override { return scale_; };

		const glm::mat4 GetPositionMatrix() const override;
		const glm::mat4 GetOrientationMatrix() const override;
		const glm::mat4 GetScaleMatrix() const override;

		const glm::mat4 GetTransformMatrix() const override;

		const glm::vec3 GetUp() const override;
		const glm::vec3 GetForward() const override;
		const glm::vec3 GetRight() const override;

		void Translate(const glm::vec3 & translation);
		void Translate(const float x, const float y, const float z);
		
		void Translate2d(const glm::vec2 & translation);
		void Translate2d(const float x, const float y);

		// Rotate by a orientation specifying the change
		void Rotate(const glm::quat & change);

		// Rotate by radians
		void Rotate(const glm::vec3 & change);
		void Rotate(const float pitch, const float yaw, const float roll);
		// Rotate by degrees
		void RotateDeg(const glm::vec3 & change);
		void RotateDeg(const float pitch, const float yaw, const float roll);

		// Rotate in the 2D plane by radians
		void Rotate2d(const float rotation);
		// Rotate in the 2D plane by degrees
		void Rotate2dDeg(const float rotation);

		void Scale(const float scalar);
		void Scale(const glm::vec3 & multiplier);
		void Scale(const float x, const float y, const float z);
		
		void Scale2d(const glm::vec2 & multiplier);
		void Scale2d(const float x, const float y);

		void SetTranslation(const glm::vec3 & translation);
		void SetTranslation(const float x, const float y, const float z);

		void SetTranslation2d(const glm::vec2 & translation);
		void SetTranslation2d(const float x, const float y);

		void SetOrientation(const glm::quat & orientation);

		void SetOrientation(const glm::vec3 & rotation);
		void SetOrientation(const float pitch, const float yaw, const float roll);

		void SetOrientationDeg(const glm::vec3 & rotation);
		void SetOrientationDeg(const float pitch, const float yaw, const float roll);

		void SetOrientation2d(const float rotation);
		void SetOrientation2dDeg(const float rotation);

		void SetScale(const float scalar);
		void SetScale(const glm::vec3 & scale);
		void SetScale(const float x, const float y, const float z);

		void SetScale2d(const glm::vec2 & scale);
		void SetScale2d(const float x, const float y);

	private:
		glm::vec3 position_;
		glm::quat orientation_;
		glm::vec3 scale_;
	};
}

