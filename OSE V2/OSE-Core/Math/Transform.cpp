#include "stdafx.h"
#include "Transform.h"

namespace ose
{
	Transform const Transform::IDENTITY { Transform() };

	Transform::Transform() : ITransform(), translation_(), orientation_(), scale_(1, 1, 1) {}

	Transform::Transform(glm::vec3 const & translation)
		: ITransform(), translation_(translation), orientation_(), scale_(1, 1, 1) {}

	Transform::Transform(glm::vec3 const & translation, glm::vec3 const & rotation_radians, glm::vec3 const & scale)
		: ITransform(), translation_(translation), orientation_(rotation_radians), scale_(scale) {}

	Transform::Transform(glm::vec3 const & translation, glm::quat const & orientation, glm::vec3 const & scale)
		: ITransform(), translation_(translation), orientation_(orientation), scale_(scale) {}

	Transform::~Transform() {}

	// Copy from ITransform to Transform object
	Transform::Transform(ITransform const & other) noexcept
		: translation_(other.GetTranslation()), orientation_(other.GetOrientation()), scale_(other.GetScale()) {}

	//copy constructors
	Transform::Transform(Transform const & other) noexcept : translation_(other.translation_), orientation_(other.orientation_), scale_(other.scale_) {}

	Transform & Transform::operator=(Transform const & other) noexcept
	{
		translation_ = other.translation_;
		orientation_ = other.orientation_;
		scale_ = other.scale_;
		return *this;
	}

	//move constructors
	Transform::Transform(Transform && other) noexcept : translation_(std::move(other.translation_)), orientation_(std::move(other.orientation_)), scale_(std::move(other.scale_)) {}

	Transform & Transform::operator=(Transform && other) noexcept
	{
		translation_ = std::move(other.translation_);
		orientation_ = std::move(other.orientation_);
		scale_ = std::move(other.scale_);
		return *this;
	}

	void Transform::Translate(glm::vec3 const & translation)
	{
		translation_ += translation;
	}

	void Transform::Translate(float x, float y, float z)
	{
		translation_.x += x;
		translation_.y += y;
		translation_.z += z;
	}
	
	void Transform::Translate2d(glm::vec2 const & translation)
	{
		translation_.x += translation.x;
		translation_.y += translation.y;
	}

	void Transform::Translate2d(float x, float y)
	{
		translation_.x += x;
		translation_.y += y;
	}

	// Rotate by a orientation specifying the change
	void Transform::Rotate(glm::quat const & change)
	{
		orientation_ *= change;
	}

	void Transform::Rotate(float pitch, float yaw, float roll)
	{
		//construct a quat from a vec3 of euler angles (in radians)
		glm::quat change { glm::vec3 { pitch, yaw, roll } };

		//cumulating quaternions -> multiply in reverse order
		orientation_ *= change;
	}

	void Transform::Rotate(glm::vec3 const & change)
	{
		//cumulating quaternions -> multiply in reverse order
		orientation_ *= glm::quat { change };
	}

	void Transform::RotateDeg(float pitch, float yaw, float roll)
	{
		//construct a quat from a vec3 of euler angles (in degrees)
		glm::quat change { glm::vec3 { glm::radians(pitch), glm::radians(yaw), glm::radians(roll) } };

		//cumulating quaternions -> multiply in reverse order
		orientation_ *= change;
	}

	void Transform::RotateDeg(glm::vec3 const & change)
	{
		//cumulating quaternions -> multiply in reverse order
		orientation_ *= glm::quat { glm::radians(change) };
	}

	// Rotate in the 2D plane by radians
	void Transform::Rotate2d(float rotation)
	{
		// Construct  a quat from a vec3 of euler angles
		glm::quat change { glm::vec3 { 0 , 0, rotation } };

		// Cumulate current orientation with change in orientation
		orientation_ *= change;
	}

	// Rotate in the 2D plane by degrees
	void Transform::Rotate2dDeg(float rotation)
	{
		// Construct  a quat from a vec3 of euler angles
		glm::quat change { glm::vec3 { 0 , 0, glm::radians(rotation) } };

		// Cumulate current orientation with change in orientation
		orientation_ *= change;
	}

	void Transform::Scale(float scalar)
	{
		scale_ *= scalar;
	}

	void Transform::Scale(glm::vec3 const & multiplier)
	{
		scale_ *= multiplier;
	}

	void Transform::Scale(float x, float y, float z)
	{
		scale_.x *= x;
		scale_.y *= y;
		scale_.z *= z;
	}

	void Transform::Scale2d(glm::vec2 const & multiplier)
	{
		scale_.x *= multiplier.x;
		scale_.y *= multiplier.y;
	}

	void Transform::Scale2d(float x, float y)
	{
		scale_.x *= x;
		scale_.y *= y;
	}


	void Transform::SetTranslation(glm::vec3 const & translation)
	{
		translation_.x = translation.x;
		translation_.y = translation.y;
		translation_.z = translation.z;
	}

	void Transform::SetTranslation(float x, float y, float z)
	{
		translation_.x = x;
		translation_.y = y;
		translation_.z = z;
	}

	void Transform::SetTranslation2d(glm::vec2 const & translation)
	{
		translation_.x = translation.x;
		translation_.y = translation.y;
		translation_.z = 0;
	}

	void Transform::SetTranslation2d(float x, float y)
	{
		translation_.x = x;
		translation_.y = y;
		translation_.z = 0;
	}


	void Transform::SetOrientation(glm::quat const & orientation)
	{
		orientation_.x = orientation.x;
		orientation_.y = orientation.y;
		orientation_.z = orientation.z;
		orientation_.w = orientation.w;
	}

	void Transform::SetOrientation(glm::vec3 const & rotation)
	{
		orientation_ = glm::quat { rotation };
	}

	void Transform::SetOrientation(float pitch, float yaw, float roll)
	{
		orientation_ = glm::quat { glm::vec3 { pitch, yaw, roll } };
	}

	void Transform::SetOrientationDeg(glm::vec3 const & rotation)
	{
		orientation_ = glm::quat { glm::vec3 { glm::radians(rotation) } };
	}

	void Transform::SetOrientationDeg(float pitch, float yaw, float roll)
	{
		orientation_ = glm::quat { glm::vec3 { glm::radians(pitch), glm::radians(yaw), glm::radians(roll) } };
	}

	void Transform::SetOrientation2d(float rotation)
	{
		orientation_ = glm::quat { glm::vec3 { 0, 0, rotation } };
	}

	void Transform::SetOrientation2dDeg(float rotation)
	{
		orientation_ = glm::quat { glm::vec3 { 0, 0, glm::radians(rotation) } };
	}


	void Transform::SetScale(float scalar)
	{
		scale_.x = scalar;
		scale_.y = scalar;
		scale_.z = scalar;
	}

	void Transform::SetScale(glm::vec3 const & scale)
	{
		scale_.x = scale.x;
		scale_.y = scale.y;
		scale_.z = scale.z;
	}

	void Transform::SetScale(float x, float y, float z)
	{
		scale_.x = x;
		scale_.y = y;
		scale_.z = z;
	}

	void Transform::SetScale2d(glm::vec2 const & scale)
	{
		scale_.x = scale.x;
		scale_.y = scale.y;
	}

	void Transform::SetScale2d(float x, float y)
	{
		scale_.x = x;
		scale_.y = y;
	}


	glm::mat4 Transform::GetTranslationMatrix() const
	{
		return glm::translate(glm::mat4(), translation_);
	}

	glm::mat4 Transform::GetOrientationMatrix() const
	{
		return glm::toMat4(orientation_);
	}

	glm::mat4 Transform::GetScaleMatrix() const
	{
		return glm::scale(glm::mat4(), scale_);
	}

	glm::mat4 Transform::GetTransformMatrix() const
	{
		return glm::translate(glm::mat4(), translation_) * glm::toMat4(orientation_) * glm::scale(glm::mat4(), scale_);
	}

	glm::mat4 Transform::GetInverseTransformMatrix() const
	{
		return glm::inverse(GetTransformMatrix());
	}
	
	glm::vec3 Transform::GetUp() const
	{
		return glm::cross(orientation_, glm::vec3(0, 1, 0));
	}

	glm::vec3 Transform::GetForward() const
	{
		return glm::cross(orientation_, glm::vec3(0, 0, -1));
	}

	glm::vec3 Transform::GetRight() const
	{
		return glm::cross(orientation_, glm::vec3(1, 0, 0));
	}
}
