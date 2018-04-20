#include "../../stdafx.h"
#include "Transform.h"

namespace origami_sheep_engine
{
	Transform::Transform() : position_(), orientation_(), scale_() {}

	Transform::Transform(const glm::vec3 & position)
		: position_(position), orientation_(), scale_() {}

	Transform::Transform(const glm::vec3 & position, const glm::vec3 & rotation_radians, const glm::vec3 & scale)
		: position_(position), orientation_(rotation_radians), scale_(scale) {}

	Transform::Transform(const glm::vec3 & position, const glm::quat & orientation, const glm::vec3 & scale)
		: position_(position), orientation_(orientation), scale_(scale) {}

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

	Transform::~Transform() {}

	void Transform::translate(const glm::vec3 & translation)
	{
		position_ += translation;
	}

	void Transform::translate(const float x, const float y, const float z)
	{
		position_.x += x;
		position_.y += y;
		position_.z += z;
	}

	void Transform::rotate(const float pitch, const float yaw, const float roll)
	{
		//construct a quat from a vec3 of euler angles (in radians)
		glm::quat change { glm::vec3 { pitch, yaw, roll } };

		//cumulating quaternions -> multiply in reverse order
		orientation_ *= change;
	}

	void Transform::rotate(const glm::vec3 & change)
	{
		//cumulating quaternions -> multiply in reverse order
		orientation_ *= glm::quat { change };
	}

	void Transform::rotateDeg(const float pitch, const float yaw, const float roll)
	{
		//construct a quat from a vec3 of euler angles (in degrees)
		glm::quat change { glm::vec3 { glm::radians(pitch), glm::radians(yaw), glm::radians(roll) } };

		//cumulating quaternions -> multiply in reverse order
		orientation_ *= change;
	}

	void Transform::rotateDeg(const glm::vec3 & change)
	{
		//cumulating quaternions -> multiply in reverse order
		orientation_ *= glm::quat { glm::radians(change) };
	}

	void Transform::scale(const float scalar)
	{
		scale_ *= scalar;
	}

	void Transform::scale(const glm::vec3 & multiplier)
	{
		scale_ *= multiplier;
	}

	void Transform::scale(const float x, const float y, const float z)
	{
		scale_.x *= x;
		scale_.y *= y;
		scale_.z *= z;
	}

	const glm::mat4 Transform::get_position_as_matrix() const
	{
		return glm::translate(glm::mat4(), position_);
	}

	const glm::mat4 Transform::get_orientation_as_matrix() const
	{
		return glm::toMat4(orientation_);
	}

	const glm::mat4 Transform::get_scale_as_matrix() const
	{
		return glm::scale(glm::mat4(), scale_);
	}

	const glm::mat4 Transform::get_transform_matrix() const
	{
		return glm::translate(glm::mat4(), position_) * glm::toMat4(orientation_) * glm::scale(glm::mat4(), scale_);
	}

	const glm::vec3 Transform::get_up() const
	{
		return glm::cross(orientation_, glm::vec3(0, 1, 0));
	}

	const glm::vec3 Transform::get_forward() const
	{
		return glm::cross(orientation_, glm::vec3(0, 0, -1));
	}

	const glm::vec3 Transform::get_right() const
	{
		return glm::cross(orientation_, glm::vec3(1, 0, 0));
	}
}
