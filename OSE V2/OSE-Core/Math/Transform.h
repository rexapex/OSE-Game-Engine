#pragma once

#include "ITransform.h"

namespace ose
{
	class Transform : public ITransform
	{
	public:
		// Constructors & destructor
		Transform();
		Transform(glm::vec3 const & translation);
		Transform(glm::vec3 const & translation, glm::vec3 const & rotation_radians, glm::vec3 const & scale);
		Transform(glm::vec3 const & translation, glm::quat const & orientation, glm::vec3 const & scale);
		~Transform();

		// Copy from ITransform to Transform object
		Transform(ITransform const & other) noexcept;

		// Copy constructors
		Transform(Transform const & other) noexcept;
		Transform & operator=(Transform const & other) noexcept;

		// Move constructors
		Transform(Transform && other) noexcept;
		Transform & operator=(Transform && other) noexcept;

		glm::vec3 const & GetTranslation() const override { return translation_; };
		glm::quat const & GetOrientation() const override { return orientation_; };
		glm::vec3 const & GetScale() const override { return scale_; };

		glm::mat4 GetTranslationMatrix() const override;
		glm::mat4 GetOrientationMatrix() const override;
		glm::mat4 GetScaleMatrix() const override;

		glm::mat4 GetTransformMatrix() const override;
		glm::mat4 GetInverseTransformMatrix() const override;

		glm::vec3 GetUp() const override;
		glm::vec3 GetForward() const override;
		glm::vec3 GetRight() const override;

		void Translate(glm::vec3 const & translation);
		void Translate(float x, float y, float z);
		
		void Translate2d(glm::vec2 const & translation);
		void Translate2d(float x, float y);

		// Rotate by a orientation specifying the change
		void Rotate(glm::quat const & change);

		// Rotate by radians
		void Rotate(glm::vec3 const & change);
		void Rotate(float pitch, float yaw, float roll);
		// Rotate by degrees
		void RotateDeg(glm::vec3 const & change);
		void RotateDeg(float pitch, float yaw, float roll);

		// Rotate in the 2D plane by radians
		void Rotate2d(float rotation);
		// Rotate in the 2D plane by degrees
		void Rotate2dDeg(float rotation);

		void Scale(float scalar);
		void Scale(glm::vec3 const & multiplier);
		void Scale(float x, float y, float z);
		
		void Scale2d(glm::vec2 const & multiplier);
		void Scale2d(float x, float y);

		void SetTranslation(glm::vec3 const & translation);
		void SetTranslation(float x, float y, float z);

		void SetTranslation2d(glm::vec2 const & translation);
		void SetTranslation2d(float x, float y);

		void SetOrientation(glm::quat const & orientation);

		void SetOrientation(glm::vec3 const & rotation);
		void SetOrientation(float pitch, float yaw, float roll);

		void SetOrientationDeg(glm::vec3 const & rotation);
		void SetOrientationDeg(float pitch, float yaw, float roll);

		void SetOrientation2d(float rotation);
		void SetOrientation2dDeg(float rotation);

		void SetScale(float scalar);
		void SetScale(glm::vec3 const & scale);
		void SetScale(float x, float y, float z);

		void SetScale2d(glm::vec2 const & scale);
		void SetScale2d(float x, float y);

	private:
		glm::vec3 translation_;
		glm::quat orientation_;
		glm::vec3 scale_;

	public:
		static Transform const IDENTITY;
	};
}

