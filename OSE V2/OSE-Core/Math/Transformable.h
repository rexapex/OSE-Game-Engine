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
			GlobalTranslate(translation);
		}

		// Modify the local and global transform of the transformable
		void Translate(const float x, const float y, const float z)
		{
			local_transform_.Translate(x, y, z);
			GlobalTranslate(x, y, z);
		}

		// Modify the local and global transform of the transformable
		void Translate2d(const glm::vec2 & translation)
		{
			local_transform_.Translate2d(translation);
			GlobalTranslate2d(translation);
		}

		// Modify the local and global transform of the transformable
		void Translate2d(const float x, const float y)
		{
			local_transform_.Translate2d(x, y);
			GlobalTranslate2d(x, y);
		}

		// Modify the local and global transform of the transformable
		void Rotate(const glm::quat & change)
		{
			local_transform_.Rotate(change);
			GlobalRotate(change);
		}

		// Modify the local and global transform of the transformable
		void Rotate(const glm::vec3 & change)
		{
			local_transform_.Rotate(change);
			GlobalRotate(change);
		}

		// Modify the local and global transform of the transformable
		void Rotate(const float pitch, const float yaw, const float roll)
		{
			local_transform_.Rotate(pitch, yaw, roll);
			GlobalRotate(pitch, yaw, roll);
		}

		// Modify the local and global transform of the transformable
		void RotateDeg(const glm::vec3 & change)
		{
			local_transform_.RotateDeg(change);
			GlobalRotateDeg(change);
		}

		// Modify the local and global transform of the transformable
		void RotateDeg(const float pitch, const float yaw, const float roll)
		{
			local_transform_.RotateDeg(pitch, yaw, roll);
			GlobalRotateDeg(pitch, yaw, roll);
		}

		// Modify the local and global transform of the transformable
		void Rotate2d(const float rotation)
		{
			local_transform_.Rotate2d(rotation);
			GlobalRotate2d(rotation);
		}

		// Modify the local and global transform of the transformable
		void Rotate2dDeg(const float rotation)
		{
			local_transform_.Rotate2dDeg(rotation);
			GlobalRotate2dDeg(rotation);
		}

		// Modify the local and global transform of the transformable
		void Scale(const float scalar)
		{
			local_transform_.Scale(scalar);
			GlobalScale(scalar);
		}

		// Modify the local and global transform of the transformable
		void Scale(const glm::vec3 & multiplier)
		{
			local_transform_.Scale(multiplier);
			GlobalScale(multiplier);
		}

		// Modify the local and global transform of the transformable
		void Scale(const float x, const float y, const float z)
		{
			local_transform_.Scale(x, y, z);
			GlobalScale(x, y, z);
		}

		// Modify the local and global transform of the transformable
		void Scale2d(const glm::vec2 & multiplier)
		{
			local_transform_.Scale2d(multiplier);
			GlobalScale2d(multiplier);
		}

		// Modify the local and global transform of the transformable
		void Scale2d(const float x, const float y)
		{
			local_transform_.Scale2d(x, y);
			GlobalScale2d(x, y);
		}

		// Modify the local and global transform of the transformable
		void SetTranslation(const glm::vec3 & translation)
		{
			local_transform_.SetTranslation(translation);
			SetGlobalTranslation();
		}

		// Modify the local and global transform of the transformable
		void SetTranslation(const float x, const float y, const float z)
		{
			local_transform_.SetTranslation(x, y, z);
			SetGlobalTranslation();
		}

		// Modify the local and global transform of the transformable
		void SetTranslation2d(const glm::vec2 & translation)
		{
			local_transform_.SetTranslation2d(translation);
			SetGlobalTranslation();
		}
		
		// Modify the local and global transform of the transformable
		void SetTranslation2d(const float x, const float y)
		{
			local_transform_.SetTranslation2d(x, y);
			SetGlobalTranslation();
		}

		// Modify the local and global transform of the transformable
		void SetOrientation(const glm::quat & orientation)
		{
			local_transform_.SetOrientation(orientation);
			SetGlobalOrientation();
		}

		// Modify the local and global transform of the transformable
		void SetOrientation(const glm::vec3 & rotation)
		{
			local_transform_.SetOrientation(rotation);
			SetGlobalOrientation();
		}

		// Modify the local and global transform of the transformable
		void SetOrientation(const float pitch, const float yaw, const float roll)
		{
			local_transform_.SetOrientation(pitch, yaw, roll);
			SetGlobalOrientation();
		}

		// Modify the local and global transform of the transformable
		void SetOrientationDeg(const glm::vec3 & rotation)
		{
			local_transform_.SetOrientationDeg(rotation);
			SetGlobalOrientation();
		}

		// Modify the local and global transform of the transformable
		void SetOrientationDeg(const float pitch, const float yaw, const float roll)
		{
			local_transform_.SetOrientationDeg(pitch, yaw, roll);
			SetGlobalOrientation();
		}

		// Modify the local and global transform of the transformable
		void SetOrientation2d(const float rotation)
		{
			local_transform_.SetOrientation2d(rotation);
			SetGlobalOrientation();
		}

		// Modify the local and global transform of the transformable
		void SetOrientation2dDeg(const float rotation)
		{
			local_transform_.SetOrientation2dDeg(rotation);
			SetGlobalOrientation();
		}

		// Modify the local and global transform of the transformable
		void SetScale(const float scalar)
		{
			local_transform_.SetScale(scalar);
			SetGlobalScale();
		}

		// Modify the local and global transform of the transformable
		void SetScale(const glm::vec3 & scale)
		{
			local_transform_.SetScale(scale);
			SetGlobalScale();
		}

		// Modify the local and global transform of the transformable
		void SetScale(const float x, const float y, const float z)
		{
			local_transform_.SetScale(x, y, z);
			SetGlobalScale();
		}

		// Modify the local and global transform of the transformable
		void SetScale2d(const glm::vec2 & scale2d)
		{
			local_transform_.SetScale2d(scale2d);
			SetGlobalScale();
		}

		// Modify the local and global transform of the transformable
		void SetScale2d(const float x, const float y)
		{
			local_transform_.SetScale2d(x, y);
			SetGlobalScale();
		}

	protected:

		Transform local_transform_;
		Transform global_transform_;

	private:

		void GlobalTranslate(const glm::vec3 & translation)
		{
			global_transform_.Translate(translation);
			for(auto & child : GetChildTransformables())
				child->GlobalTranslate(translation);
		}

		void GlobalTranslate(const float x, const float y, const float z)
		{
			global_transform_.Translate(x, y, z);
			for(auto & child : GetChildTransformables())
				child->GlobalTranslate(x, y, z);
		}

		void GlobalTranslate2d(const glm::vec2 & translation)
		{
			global_transform_.Translate2d(translation);
			for(auto & child : GetChildTransformables())
				child->GlobalTranslate2d(translation);
		}

		void GlobalTranslate2d(const float x, const float y)
		{
			global_transform_.Translate2d(x, y);
			for(auto & child : GetChildTransformables())
				child->GlobalTranslate2d(x, y);
		}

		void GlobalRotate(const glm::quat & change)
		{
			global_transform_.Rotate(change);
			for(auto & child : GetChildTransformables())
				child->GlobalRotate(change);
		}

		void GlobalRotate(const glm::vec3 & change)
		{
			global_transform_.Rotate(change);
			for(auto & child : GetChildTransformables())
				child->GlobalRotate(change);
		}

		void GlobalRotate(const float pitch, const float yaw, const float roll)
		{
			global_transform_.Rotate(pitch, yaw, roll);
			for(auto & child : GetChildTransformables())
				child->GlobalRotate(pitch, yaw, roll);
		}

		void GlobalRotateDeg(const glm::vec3 & change)
		{
			global_transform_.RotateDeg(change);
			for(auto & child : GetChildTransformables())
				child->GlobalRotateDeg(change);
		}

		void GlobalRotateDeg(const float pitch, const float yaw, const float roll)
		{
			global_transform_.RotateDeg(pitch, yaw, roll);
			for(auto & child : GetChildTransformables())
				child->GlobalRotateDeg(pitch, yaw, roll);
		}

		void GlobalRotate2d(const float rotation)
		{
			global_transform_.Rotate2d(rotation);
			for(auto & child : GetChildTransformables())
				child->GlobalRotate2d(rotation);
		}

		void GlobalRotate2dDeg(const float rotation)
		{
			global_transform_.Rotate2dDeg(rotation);
			for(auto & child : GetChildTransformables())
				child->GlobalRotate2dDeg(rotation);
		}

		void GlobalScale(const float scalar)
		{
			global_transform_.Scale(scalar);
			for(auto & child : GetChildTransformables())
				child->GlobalScale(scalar);
		}

		void GlobalScale(const glm::vec3 & multiplier)
		{
			global_transform_.Scale(multiplier);
			for(auto & child : GetChildTransformables())
				child->GlobalScale(multiplier);
		}

		void GlobalScale(const float x, const float y, const float z)
		{
			global_transform_.Scale(x, y, z);
			for(auto & child : GetChildTransformables())
				child->GlobalScale(x, y, z);
		}

		void GlobalScale2d(const glm::vec2 & multiplier)
		{
			global_transform_.Scale2d(multiplier);
			for(auto & child : GetChildTransformables())
				child->GlobalScale2d(multiplier);
		}

		void GlobalScale2d(const float x, const float y)
		{
			global_transform_.Scale2d(x, y);
			for(auto & child : GetChildTransformables())
				child->GlobalScale2d(x, y);
		}

		void SetGlobalTranslation()
		{
			Transformable * parent { GetParentTransformable() };
			if(parent)
				global_transform_.SetTranslation(parent->GetGlobalTransform().GetPosition() + local_transform_.GetPosition());
			else
				global_transform_.SetTranslation(local_transform_.GetPosition());
			for(auto & child : GetChildTransformables())
				child->SetGlobalTranslation();
		}

		void SetGlobalOrientation()
		{
			Transformable * parent { GetParentTransformable() };
			if(parent)
				global_transform_.SetOrientation(parent->GetGlobalTransform().GetOrientation() * local_transform_.GetOrientation());
			else
				global_transform_.SetOrientation(local_transform_.GetOrientation());
			for(auto & child : GetChildTransformables())
				child->SetGlobalOrientation();
		}

		void SetGlobalScale()
		{
			Transformable * parent { GetParentTransformable() };
			if(parent)
				global_transform_.SetScale(parent->GetGlobalTransform().GetScale() * local_transform_.GetScale());
			else
				global_transform_.SetScale(local_transform_.GetScale());
			for(auto & child : GetChildTransformables())
				child->SetGlobalScale();
		}

		// Get a list of child transformable elements
		virtual const std::vector<T> & GetChildTransformables() const = 0;

		// Get a pointer to the parent transformable element
		virtual Transformable * GetParentTransformable() const = 0;
	};
}

