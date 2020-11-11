#pragma once

#include "ITransform.h"
#include "Transform.h"

namespace ose
{
	// NOTE - If extending Transformable, T should be a pointer type
	template <class T>
	class Transformable
	{
	public:
		Transformable() {}
		virtual ~Transformable() {}

		// Copy/move constructors
		Transformable(const Transformable & other) noexcept = default;
		Transformable & operator=(const Transformable & other) noexcept = default;
		Transformable(Transformable && other) noexcept = default;
		Transformable & operator=(Transformable && other) noexcept = default;

		// Accessor methods for retrieving const references to transform objects
		ITransform const & GetLocalTransform() const { return local_transform_; }
		ITransform const & GetGlobalTransform() const { return global_transform_; }


		// Modify the local and global transform of the transformable
		void Translate(const glm::vec3 & translation)
		{
			local_transform_.Translate(translation);
			TranslateParent(translation);
		}

		// Modify the local and global transform of the transformable
		void Translate(const float x, const float y, const float z)
		{
			local_transform_.Translate(x, y, z);
			TranslateParent(x, y, z);
		}

		// Modify the local and global transform of the transformable
		void Translate2d(const glm::vec2 & translation)
		{
			local_transform_.Translate2d(translation);
			TranslateParent(translation);
		}

		// Modify the local and global transform of the transformable
		void Translate2d(const float x, const float y)
		{
			local_transform_.Translate2d(x, y);
			Translate2dParent(x, y);
		}


		// Modify the local and global transform of the transformable
		void Rotate(const glm::quat & change)
		{
			local_transform_.Rotate(change);
			RotateParent(change);
		}

		// Modify the local and global transform of the transformable
		void Rotate(const glm::vec3 & change)
		{
			local_transform_.Rotate(change);
			RotateParent(change);
		}

		// Modify the local and global transform of the transformable
		void Rotate(const float pitch, const float yaw, const float roll)
		{
			local_transform_.Rotate(pitch, yaw, roll);
			RotateParent(pitch, yaw, roll);
		}

		// Modify the local and global transform of the transformable
		void RotateDeg(const glm::vec3 & change)
		{
			local_transform_.RotateDeg(change);
			RotateDegParent(change);
		}

		// Modify the local and global transform of the transformable
		void RotateDeg(const float pitch, const float yaw, const float roll)
		{
			local_transform_.RotateDeg(pitch, yaw, roll);
			RotateDegParent(pitch, yaw, roll);
		}

		// Modify the local and global transform of the transformable
		void Rotate2d(const float rotation)
		{
			local_transform_.Rotate2d(rotation);
			Rotate2dParent(rotation);
		}

		// Modify the local and global transform of the transformable
		void Rotate2dDeg(const float rotation)
		{
			local_transform_.Rotate2dDeg(rotation);
			Rotate2dDegParent(rotation);
		}


		// Modify the local and global transform of the transformable
		void Scale(const float scalar)
		{
			local_transform_.Scale(scalar);
			ScaleParent(scalar);
		}

		// Modify the local and global transform of the transformable
		void Scale(const glm::vec3 & multiplier)
		{
			local_transform_.Scale(multiplier);
			ScaleParent(multiplier);
		}

		// Modify the local and global transform of the transformable
		void Scale(const float x, const float y, const float z)
		{
			local_transform_.Scale(x, y, z);
			ScaleParent(x, y, z);
		}

		// Modify the local and global transform of the transformable
		void Scale2d(const glm::vec2 & multiplier)
		{
			local_transform_.Scale2d(multiplier);
			Scale2dParent(multiplier);
		}

		// Modify the local and global transform of the transformable
		void Scale2d(const float x, const float y)
		{
			local_transform_.Scale2d(x, y);
			Scale2dParent(x, y);
		}

		// Modify the local and global transform of the transformable
		void SetTranslation(const glm::vec3 & translation)
		{
			glm::vec3 const & b { local_transform_.GetPosition() };
			local_transform_.SetTranslation(translation);
			TranslateParent(translation - b);
		}

		// Modify the local and global transform of the transformable
		void SetTranslation(const float x, const float y, const float z)
		{
			glm::vec3 const & b { local_transform_.GetPosition() };
			local_transform_.SetTranslation(x, y, z);
			TranslateParent(x - b.x, y - b.y, z - b.z);
		}

		// Modify the local and global transform of the transformable
		void SetTranslation2d(const glm::vec2 & translation)
		{
			glm::vec3 const & b { local_transform_.GetPosition() };
			local_transform_.SetTranslation2d(translation);
			Translate2dParent(translation - b);
		}
		
		// Modify the local and global transform of the transformable
		void SetTranslation2d(const float x, const float y)
		{
			glm::vec3 const & b { local_transform_.GetPosition() };
			local_transform_.SetTranslation2d(x, y);
			Translate2dParent(x - b.x, y - b.y);
		}

		// Modify the local and global transform of the transformable
		void SetOrientation(const glm::quat & orientation)
		{
			glm::quat const & b { glm::inverse(local_transform_.GetOrientation()) };
			local_transform_.SetOrientation(orientation);
			RotateParent(orientation * b);
		}

		// Modify the local and global transform of the transformable
		void SetOrientation(const glm::vec3 & rotation)
		{
			glm::quat const & b { glm::inverse(local_transform_.GetOrientation()) };
			local_transform_.SetOrientation(rotation);
			RotateParent(b);
			RotateParent(rotation);
		}

		// Modify the local and global transform of the transformable
		void SetOrientation(const float pitch, const float yaw, const float roll)
		{
			glm::quat const & b { glm::inverse(local_transform_.GetOrientation()) };
			local_transform_.SetOrientation(pitch, yaw, roll);
			RotateParent(b);
			RotateParent(pitch, yaw, roll);
		}

		// Modify the local and global transform of the transformable
		void SetOrientationDeg(const glm::vec3 & rotation)
		{
			glm::quat const & b { glm::inverse(local_transform_.GetOrientation()) };
			local_transform_.SetOrientationDeg(rotation);
			RotateParent(b);
			RotateDegParent(rotation);
		}

		// Modify the local and global transform of the transformable
		void SetOrientationDeg(const float pitch, const float yaw, const float roll)
		{
			glm::quat const & b { glm::inverse(local_transform_.GetOrientation()) };
			local_transform_.SetOrientationDeg(pitch, yaw, roll);
			RotateParent(b);
			RotateDegParent(pitch, yaw, roll);
		}

		// Modify the local and global transform of the transformable
		void SetOrientation2d(const float rotation)
		{
			glm::quat const & b { glm::inverse(local_transform_.GetOrientation()) };
			local_transform_.SetOrientation2d(rotation);
			RotateParent(b);
			Rotate2dParent(rotation);
		}

		// Modify the local and global transform of the transformable
		void SetOrientation2dDeg(const float rotation)
		{
			glm::quat const & b { glm::inverse(local_transform_.GetOrientation()) };
			local_transform_.SetOrientation2dDeg(rotation);
			RotateParent(b);
			Rotate2dDegParent(rotation);
		}

		// Modify the local and global transform of the transformable
		void SetScale(const float scalar)
		{
			glm::vec3 const & b { local_transform_.GetScale() };
			glm::vec3 inverse { b.x == 0 ? 1 : 1/b.x, b.y == 0 ? 1 : 1/b.y, b.z == 0 ? 1 : 1/b.z };
			local_transform_.SetScale(scalar);
			ScaleParent(inverse * scalar);
		}

		// Modify the local and global transform of the transformable
		void SetScale(const glm::vec3 & scale)
		{
			glm::vec3 const & b { local_transform_.GetScale() };
			glm::vec3 inverse { b.x == 0 ? 1 : 1/b.x, b.y == 0 ? 1 : 1/b.y, b.z == 0 ? 1 : 1/b.z };
			local_transform_.SetScale(scale);
			ScaleParent(inverse * scale);
		}

		// Modify the local and global transform of the transformable
		void SetScale(const float x, const float y, const float z)
		{
			glm::vec3 scale { x, y, z };
			glm::vec3 const & b { local_transform_.GetScale() };
			glm::vec3 inverse { b.x == 0 ? 1 : 1/b.x, b.y == 0 ? 1 : 1/b.y, b.z == 0 ? 1 : 1/b.z };
			local_transform_.SetScale(scale);
			ScaleParent(inverse * scale);
		}

		// Modify the local and global transform of the transformable
		void SetScale2d(const glm::vec2 & scale2d)
		{
			glm::vec3 scale { scale2d.x, scale2d.y, 1 };
			glm::vec3 const & b { local_transform_.GetScale() };
			glm::vec3 inverse { b.x == 0 ? 1 : 1/b.x, b.y == 0 ? 1 : 1/b.y, 1 };
			local_transform_.SetScale(scale);
			ScaleParent(inverse * scale);
		}

		// Modify the local and global transform of the transformable
		void SetScale2d(const float x, const float y)
		{
			glm::vec3 scale { x, y, 1 };
			glm::vec3 const & b { local_transform_.GetScale() };
			glm::vec3 inverse { b.x == 0 ? 1 : 1/b.x, b.y == 0 ? 1 : 1/b.y, 1 };
			local_transform_.SetScale(scale);
			ScaleParent(inverse * scale);
		}

	protected:

		Transform local_transform_;
		Transform global_transform_;

	private:

		// Modify just the global transform of the transformable
		void TranslateParent(const glm::vec3 & translation)
		{
			global_transform_.Translate(translation);
			for(auto & child : GetChildTransformables())
			{
				child->TranslateParent(translation);
			}
		}

		// Modify just the global transform of the transformable
		void TranslateParent(const float x, const float y, const float z)
		{
			global_transform_.Translate(x, y, z);
			for(auto & child : GetChildTransformables())
			{
				child->TranslateParent(x, y, z);
			}
		}

		// Modify just the global transform of the transformable
		void Translate2dParent(const glm::vec2 & translation)
		{
			global_transform_.Translate2d(translation);
			for(auto & child : GetChildTransformables())
			{
				child->Translate2dParent(translation);
			}
		}

		// Modify just the global transform of the transformable
		void Translate2dParent(const float x, const float y)
		{
			global_transform_.Translate2d(x, y);
			for(auto & child : GetChildTransformables())
			{
				child->Translate2dParent(x, y);
			}
		}


		// Modify just the global transform of the transformable
		void RotateParent(const glm::quat & change)
		{
			global_transform_.Rotate(change);
			for(auto & child : GetChildTransformables())
			{
				child->RotateParent(change);
			}
		}

		// Modify just the global transform of the transformable
		void RotateParent(const glm::vec3 & change)
		{
			global_transform_.Rotate(change);
			for(auto & child : GetChildTransformables())
			{
				child->RotateParent(change);
			}
		}

		// Modify just the global transform of the transformable
		void RotateParent(const float pitch, const float yaw, const float roll)
		{
			global_transform_.Rotate(pitch, yaw, roll);
			for(auto & child : GetChildTransformables())
			{
				child->RotateParent(pitch, yaw, roll);
			}
		}

		// Modify just the global transform of the transformable
		void RotateDegParent(const glm::vec3 & change)
		{
			global_transform_.RotateDeg(change);
			for(auto & child : GetChildTransformables())
			{
				child->RotateDegParent(change);
			}
		}

		// Modify just the global transform of the transformable
		void RotateDegParent(const float pitch, const float yaw, const float roll)
		{
			global_transform_.RotateDeg(pitch, yaw, roll);
			for(auto & child : GetChildTransformables())
			{
				child->RotateDegParent(pitch, yaw, roll);
			}
		}

		// Modify the local and global transform of the transformable
		void Rotate2dParent(const float rotation)
		{
			global_transform_.Rotate2d(rotation);
			for(auto & child : GetChildTransformables())
			{
				child->Rotate2dParent(rotation);
			}
		}

		// Modify the local and global transform of the transformable
		void Rotate2dDegParent(const float rotation)
		{
			global_transform_.Rotate2dDeg(rotation);
			for(auto & child : GetChildTransformables())
			{
				child->Rotate2dDegParent(rotation);
			}
		}


		// Modify just the global transform of the transformable
		void ScaleParent(const float scalar)
		{
			global_transform_.Scale(scalar);
			for(auto & child : GetChildTransformables())
			{
				child->ScaleParent(scalar);
			}
		}

		// Modify just the global transform of the transformable
		void ScaleParent(const glm::vec3 & multiplier)
		{
			global_transform_.Scale(multiplier);
			for(auto & child : GetChildTransformables())
			{
				child->ScaleParent(multiplier);
			}
		}

		// Modify just the global transform of the transformable
		void ScaleParent(const float x, const float y, const float z)
		{
			global_transform_.Scale(x, y, z);
			for(auto & child : GetChildTransformables())
			{
				child->ScaleParent(x, y, z);
			}
		}

		// Modify the local and global transform of the transformable
		void Scale2dParent(const glm::vec2 & multiplier)
		{
			global_transform_.Scale2d(multiplier);
			for(auto & child : GetChildTransformables())
			{
				child->Scale2dParent(multiplier);
			}
		}

		// Modify the local and global transform of the transformable
		void Scale2dParent(const float x, const float y)
		{
			global_transform_.Scale2d(x, y);
			for(auto & child : GetChildTransformables())
			{
				child->Scale2dParent(x, y);
			}
		}

		// Get a list of child transformable elements
		virtual const std::vector<T> & GetChildTransformables() const = 0;

		// Get a pointer to the parent transformable element
		virtual Transformable * GetParentTransformable() const = 0;
	};
}

