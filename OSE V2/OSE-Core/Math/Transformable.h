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
		Transformable(Transformable const & other) noexcept = default;
		Transformable & operator=(Transformable const & other) noexcept = default;
		Transformable(Transformable && other) noexcept = default;
		Transformable & operator=(Transformable && other) noexcept = default;

		// Accessor methods for retrieving const references to transform objects
		ITransform const & GetLocalTransform() const { return local_transform_; }
		ITransform const & GetGlobalTransform() const { return global_transform_; }

		// Modify the local and global transform of the transformable
		void Translate(glm::vec3 const & translation)
		{
			local_transform_.Translate(translation);
			GlobalTranslate(translation);
		}

		// Modify the local and global transform of the transformable
		void Translate(float x, float y, float z)
		{
			local_transform_.Translate(x, y, z);
			GlobalTranslate(x, y, z);
		}

		// Modify the local and global transform of the transformable
		void Translate2d(glm::vec2 const & translation)
		{
			local_transform_.Translate2d(translation);
			GlobalTranslate2d(translation);
		}

		// Modify the local and global transform of the transformable
		void Translate2d(float x, float y)
		{
			local_transform_.Translate2d(x, y);
			GlobalTranslate2d(x, y);
		}

		// Modify the local and global transform of the transformable
		void Rotate(glm::quat const & change)
		{
			local_transform_.Rotate(change);
			GlobalRotate(change);
		}

		// Modify the local and global transform of the transformable
		void Rotate(glm::vec3 const & change)
		{
			local_transform_.Rotate(change);
			GlobalRotate(change);
		}

		// Modify the local and global transform of the transformable
		void Rotate(float pitch, float yaw, float roll)
		{
			local_transform_.Rotate(pitch, yaw, roll);
			GlobalRotate(pitch, yaw, roll);
		}

		// Modify the local and global transform of the transformable
		void RotateDeg(glm::vec3 const & change)
		{
			local_transform_.RotateDeg(change);
			GlobalRotateDeg(change);
		}

		// Modify the local and global transform of the transformable
		void RotateDeg(float pitch, float yaw, float roll)
		{
			local_transform_.RotateDeg(pitch, yaw, roll);
			GlobalRotateDeg(pitch, yaw, roll);
		}

		// Modify the local and global transform of the transformable
		void Rotate2d(float rotation)
		{
			local_transform_.Rotate2d(rotation);
			GlobalRotate2d(rotation);
		}

		// Modify the local and global transform of the transformable
		void Rotate2dDeg(float rotation)
		{
			local_transform_.Rotate2dDeg(rotation);
			GlobalRotate2dDeg(rotation);
		}

		// Modify the local and global transform of the transformable
		void Scale(float scalar)
		{
			local_transform_.Scale(scalar);
			GlobalScale(scalar);
		}

		// Modify the local and global transform of the transformable
		void Scale(glm::vec3 const & multiplier)
		{
			local_transform_.Scale(multiplier);
			GlobalScale(multiplier);
		}

		// Modify the local and global transform of the transformable
		void Scale(float x, float y, float z)
		{
			local_transform_.Scale(x, y, z);
			GlobalScale(x, y, z);
		}

		// Modify the local and global transform of the transformable
		void Scale2d(glm::vec2 const & multiplier)
		{
			local_transform_.Scale2d(multiplier);
			GlobalScale2d(multiplier);
		}

		// Modify the local and global transform of the transformable
		void Scale2d(float x, float y)
		{
			local_transform_.Scale2d(x, y);
			GlobalScale2d(x, y);
		}

		// Modify the local and global transform of the transformable
		void SetTranslation(glm::vec3 const & translation)
		{
			local_transform_.SetTranslation(translation);
			SetGlobalTranslation();
		}

		// Modify the local and global transform of the transformable
		void SetTranslation(float x, float y, float z)
		{
			local_transform_.SetTranslation(x, y, z);
			SetGlobalTranslation();
		}

		// Modify the local and global transform of the transformable
		void SetTranslation2d(glm::vec2 const & translation)
		{
			local_transform_.SetTranslation2d(translation);
			SetGlobalTranslation();
		}
		
		// Modify the local and global transform of the transformable
		void SetTranslation2d(float x, float y)
		{
			local_transform_.SetTranslation2d(x, y);
			SetGlobalTranslation();
		}

		// Modify the local and global transform of the transformable
		void SetOrientation(glm::quat const & orientation)
		{
			local_transform_.SetOrientation(orientation);
			SetGlobalOrientation();
		}

		// Modify the local and global transform of the transformable
		void SetOrientation(glm::vec3 const & rotation)
		{
			local_transform_.SetOrientation(rotation);
			SetGlobalOrientation();
		}

		// Modify the local and global transform of the transformable
		void SetOrientation(float pitch, float yaw, float roll)
		{
			local_transform_.SetOrientation(pitch, yaw, roll);
			SetGlobalOrientation();
		}

		// Modify the local and global transform of the transformable
		void SetOrientationDeg(glm::vec3 const & rotation)
		{
			local_transform_.SetOrientationDeg(rotation);
			SetGlobalOrientation();
		}

		// Modify the local and global transform of the transformable
		void SetOrientationDeg(float pitch, float yaw, float roll)
		{
			local_transform_.SetOrientationDeg(pitch, yaw, roll);
			SetGlobalOrientation();
		}

		// Modify the local and global transform of the transformable
		void SetOrientation2d(float rotation)
		{
			local_transform_.SetOrientation2d(rotation);
			SetGlobalOrientation();
		}

		// Modify the local and global transform of the transformable
		void SetOrientation2dDeg(float rotation)
		{
			local_transform_.SetOrientation2dDeg(rotation);
			SetGlobalOrientation();
		}

		// Modify the local and global transform of the transformable
		void SetScale(float scalar)
		{
			local_transform_.SetScale(scalar);
			SetGlobalScale();
		}

		// Modify the local and global transform of the transformable
		void SetScale(glm::vec3 const & scale)
		{
			local_transform_.SetScale(scale);
			SetGlobalScale();
		}

		// Modify the local and global transform of the transformable
		void SetScale(float x, float y, float z)
		{
			local_transform_.SetScale(x, y, z);
			SetGlobalScale();
		}

		// Modify the local and global transform of the transformable
		void SetScale2d(glm::vec2 const & scale2d)
		{
			local_transform_.SetScale2d(scale2d);
			SetGlobalScale();
		}

		// Modify the local and global transform of the transformable
		void SetScale2d(float x, float y)
		{
			local_transform_.SetScale2d(x, y);
			SetGlobalScale();
		}

	protected:

		// Reset the global transform, i.e. set the global transform to the parent's global transform followed by the local transform
		void ResetGlobalTransform()
		{
			SetGlobalTranslation();
			SetGlobalOrientation();
			SetGlobalScale();
		}

		// Get a list of child transformable elements
		virtual std::vector<T> const & GetChildTransformables() const = 0;

		// Get a pointer to the parent transformable element
		virtual Transformable * GetParentTransformable() const = 0;

	private:

		void GlobalTranslate(glm::vec3 const & translation)
		{
			global_transform_.Translate(translation);
			for(auto & child : GetChildTransformables())
				child->GlobalTranslate(translation);
		}

		void GlobalTranslate(float x, float y, float z)
		{
			global_transform_.Translate(x, y, z);
			for(auto & child : GetChildTransformables())
				child->GlobalTranslate(x, y, z);
		}

		void GlobalTranslate2d(glm::vec2 const & translation)
		{
			global_transform_.Translate2d(translation);
			for(auto & child : GetChildTransformables())
				child->GlobalTranslate2d(translation);
		}

		void GlobalTranslate2d(float x, float y)
		{
			global_transform_.Translate2d(x, y);
			for(auto & child : GetChildTransformables())
				child->GlobalTranslate2d(x, y);
		}

		void GlobalRotate(glm::quat const & change)
		{
			global_transform_.Rotate(change);
			for(auto & child : GetChildTransformables())
				child->GlobalRotate(change);
		}

		void GlobalRotate(glm::vec3 const & change)
		{
			global_transform_.Rotate(change);
			for(auto & child : GetChildTransformables())
				child->GlobalRotate(change);
		}

		void GlobalRotate(float pitch, float yaw, float roll)
		{
			global_transform_.Rotate(pitch, yaw, roll);
			for(auto & child : GetChildTransformables())
				child->GlobalRotate(pitch, yaw, roll);
		}

		void GlobalRotateDeg(glm::vec3 const & change)
		{
			global_transform_.RotateDeg(change);
			for(auto & child : GetChildTransformables())
				child->GlobalRotateDeg(change);
		}

		void GlobalRotateDeg(float pitch, float yaw, float roll)
		{
			global_transform_.RotateDeg(pitch, yaw, roll);
			for(auto & child : GetChildTransformables())
				child->GlobalRotateDeg(pitch, yaw, roll);
		}

		void GlobalRotate2d(float rotation)
		{
			global_transform_.Rotate2d(rotation);
			for(auto & child : GetChildTransformables())
				child->GlobalRotate2d(rotation);
		}

		void GlobalRotate2dDeg(float rotation)
		{
			global_transform_.Rotate2dDeg(rotation);
			for(auto & child : GetChildTransformables())
				child->GlobalRotate2dDeg(rotation);
		}

		void GlobalScale(float scalar)
		{
			global_transform_.Scale(scalar);
			for(auto & child : GetChildTransformables())
				child->GlobalScale(scalar);
		}

		void GlobalScale(glm::vec3 const & multiplier)
		{
			global_transform_.Scale(multiplier);
			for(auto & child : GetChildTransformables())
				child->GlobalScale(multiplier);
		}

		void GlobalScale(float x, float y, float z)
		{
			global_transform_.Scale(x, y, z);
			for(auto & child : GetChildTransformables())
				child->GlobalScale(x, y, z);
		}

		void GlobalScale2d(glm::vec2 const & multiplier)
		{
			global_transform_.Scale2d(multiplier);
			for(auto & child : GetChildTransformables())
				child->GlobalScale2d(multiplier);
		}

		void GlobalScale2d(float x, float y)
		{
			global_transform_.Scale2d(x, y);
			for(auto & child : GetChildTransformables())
				child->GlobalScale2d(x, y);
		}

		void SetGlobalTranslation()
		{
			Transformable * parent { GetParentTransformable() };
			if(parent)
				global_transform_.SetTranslation(parent->GetGlobalTransform().GetTranslation() + local_transform_.GetTranslation());
			else
				global_transform_.SetTranslation(local_transform_.GetTranslation());
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

	protected:
		Transform local_transform_;
		Transform global_transform_;
	};
}

