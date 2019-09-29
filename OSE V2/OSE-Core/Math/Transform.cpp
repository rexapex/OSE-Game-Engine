#include "stdafx.h"
#include "Transform.h"

namespace ose::math
{
	Transform::Transform() : ITransform(), position_(), orientation_(), scale_(1, 1, 1) {}

	Transform::Transform(const glm::vec3 & position)
		: ITransform(), position_(position), orientation_(), scale_(1, 1, 1) {}

	Transform::Transform(const glm::vec3 & position, const glm::vec3 & rotation_radians, const glm::vec3 & scale)
		: ITransform(), position_(position), orientation_(rotation_radians), scale_(scale) {}

	Transform::Transform(const glm::vec3 & position, const glm::quat & orientation, const glm::vec3 & scale)
		: ITransform(), position_(position), orientation_(orientation), scale_(scale) {}

	Transform::~Transform() {}

	// Copy from ITransform to Transform object
	Transform::Transform(const ITransform & other) noexcept
		: position_(other.GetPosition()), orientation_(other.GetOrientation()), scale_(other.GetScale()) {}

	//copy constructors
	Transform::Transform(const Transform & other) noexcept : position_(other.position_), orientation_(other.orientation_), scale_(other.scale_) {}

	Transform & Transform::operator=(const Transform & other) noexcept
	{
		position_ = other.position_;
		orientation_ = other.orientation_;
		scale_ = other.scale_;
		return *this;
	}

	//move constructors
	Transform::Transform(Transform && other) noexcept : position_(std::move(other.position_)), orientation_(std::move(other.orientation_)), scale_(std::move(other.scale_)) {}

	Transform & Transform::operator=(Transform && other) noexcept
	{
		position_ = std::move(other.position_);
		orientation_ = std::move(other.orientation_);
		scale_ = std::move(other.scale_);
		return *this;
	}

	void Transform::Translate(const glm::vec3 & translation)
	{
		position_ += translation;
	}

	void Transform::Translate(const float x, const float y, const float z)
	{
		position_.x += x;
		position_.y += y;
		position_.z += z;
	}

	void Transform::Rotate(const float pitch, const float yaw, const float roll)
	{
		//construct a quat from a vec3 of euler angles (in radians)
		glm::quat change { glm::vec3 { pitch, yaw, roll } };

		//cumulating quaternions -> multiply in reverse order
		orientation_ *= change;
	}

	void Transform::Rotate(const glm::vec3 & change)
	{
		//cumulating quaternions -> multiply in reverse order
		orientation_ *= glm::quat { change };
	}

	void Transform::RotateDeg(const float pitch, const float yaw, const float roll)
	{
		//construct a quat from a vec3 of euler angles (in degrees)
		glm::quat change { glm::vec3 { glm::radians(pitch), glm::radians(yaw), glm::radians(roll) } };

		//cumulating quaternions -> multiply in reverse order
		orientation_ *= change;
	}

	void Transform::RotateDeg(const glm::vec3 & change)
	{
		//cumulating quaternions -> multiply in reverse order
		orientation_ *= glm::quat { glm::radians(change) };
	}

	void Transform::Scale(const float scalar)
	{
		scale_ *= scalar;
	}

	void Transform::Scale(const glm::vec3 & multiplier)
	{
		scale_ *= multiplier;
	}

	void Transform::Scale(const float x, const float y, const float z)
	{
		scale_.x *= x;
		scale_.y *= y;
		scale_.z *= z;
	}

	const glm::mat4 Transform::GetPositionMatrix() const
	{
		return glm::translate(glm::mat4(), position_);
	}

	const glm::mat4 Transform::GetOrientationMatrix() const
	{
		return glm::toMat4(orientation_);
	}

	const glm::mat4 Transform::GetScaleMatrix() const
	{
		return glm::scale(glm::mat4(), scale_);
	}

	const glm::mat4 Transform::GetTransformMatrix() const
	{
		return glm::scale(glm::mat4(), scale_) * glm::toMat4(orientation_) * glm::translate(glm::mat4(), position_);
	}

	const glm::vec3 Transform::GetUp() const
	{
		return glm::cross(orientation_, glm::vec3(0, 1, 0));
	}

	const glm::vec3 Transform::GetForward() const
	{
		return glm::cross(orientation_, glm::vec3(0, 0, -1));
	}

	const glm::vec3 Transform::GetRight() const
	{
		return glm::cross(orientation_, glm::vec3(1, 0, 0));
	}
}
