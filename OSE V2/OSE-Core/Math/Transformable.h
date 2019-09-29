#pragma once

#include "ITransform.h"
#include "Transform.h"

namespace ose::math
{
	// If extending, be sure to include TRANSFORMABLE_DEFINITION macro within sub-class
	// IMPORTANT - Set T to the child class which is extending Transformable
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

		// Modify the local and global transform of the entity
		void Translate(const glm::vec3 & translation)
		{
			local_transform_.Translate(translation);
			global_transform_.Translate(translation);
			for(auto & child : GetChildTransformables())
			{
				child->TranslateParent(translation);
			}
		}

		// Modify the local and global transform of the entity
		void Translate(const float x, const float y, const float z)
		{
			local_transform_.Translate(x, y, z);
			global_transform_.Translate(x, y, z);
			for(auto & child : GetChildTransformables())
			{
				child->TranslateParent(x, y, z);
			}
		}

		// Modify the local and global transform of the entity
		void Rotate(const glm::vec3 & change)
		{
			local_transform_.Rotate(change);
			global_transform_.Rotate(change);
			for(auto & child : GetChildTransformables())
			{
				child->RotateParent(change);
			}
		}

		// Modify the local and global transform of the entity
		void Rotate(const float pitch, const float yaw, const float roll)
		{
			local_transform_.Rotate(pitch, yaw, roll);
			global_transform_.Rotate(pitch, yaw, roll);
			for(auto & child : GetChildTransformables())
			{
				child->RotateParent(pitch, yaw, roll);
			}
		}

		// Modify the local and global transform of the entity
		void RotateDeg(const glm::vec3 & change)
		{
			local_transform_.RotateDeg(change);
			global_transform_.RotateDeg(change);
			for(auto & child : GetChildTransformables())
			{
				child->RotateDegParent(change);
			}
		}

		// Modify the local and global transform of the entity
		void RotateDeg(const float pitch, const float yaw, const float roll)
		{
			local_transform_.RotateDeg(pitch, yaw, roll);
			global_transform_.RotateDeg(pitch, yaw, roll);
			for(auto & child : GetChildTransformables())
			{
				child->RotateDegParent(pitch, yaw, roll);
			}
		}

		// Modify the local and global transform of the entity
		void Scale(const float scalar)
		{
			local_transform_.Scale(scalar);
			global_transform_.Scale(scalar);
			for(auto & child : GetChildTransformables())
			{
				child->ScaleParent(scalar);
			}
		}

		// Modify the local and global transform of the entity
		void Scale(const glm::vec3 & multiplier)
		{
			local_transform_.Scale(multiplier);
			global_transform_.Scale(multiplier);
			for(auto & child : GetChildTransformables())
			{
				child->ScaleParent(multiplier);
			}
		}


		// Modify the local and global transform of the entity
		void Scale(const float x, const float y, const float z)
		{
			local_transform_.Scale(x, y, z);
			global_transform_.Scale(x, y, z);
			for(auto & child : GetChildTransformables())
			{
				child->ScaleParent(x, y, z);
			}
		}
		
	protected:

		Transform local_transform_;
		Transform global_transform_;

	private:

		// Modify just the global transform of the entity
		void TranslateParent(const glm::vec3 & translation)
		{
			global_transform_.Translate(translation);
			for(auto & child : GetChildTransformables())
			{
				child->TranslateParent(translation);
			}
		}

		// Modify just the global transform of the entity
		void TranslateParent(const float x, const float y, const float z)
		{
			global_transform_.Translate(x, y, z);
			for(auto & child : GetChildTransformables())
			{
				child->TranslateParent(x, y, z);
			}
		}

		// Modify just the global transform of the entity
		void RotateParent(const glm::vec3 & change)
		{
			global_transform_.Rotate(change);
			for(auto & child : GetChildTransformables())
			{
				child->RotateParent(change);
			}
		}

		// Modify just the global transform of the entity
		void RotateParent(const float pitch, const float yaw, const float roll)
		{
			global_transform_.Rotate(pitch, yaw, roll);
			for(auto & child : GetChildTransformables())
			{
				child->RotateParent(pitch, yaw, roll);
			}
		}

		// Modify just the global transform of the entity
		void RotateDegParent(const glm::vec3 & change)
		{
			global_transform_.RotateDeg(change);
			for(auto & child : GetChildTransformables())
			{
				child->RotateDegParent(change);
			}
		}

		// Modify just the global transform of the entity
		void RotateDegParent(const float pitch, const float yaw, const float roll)
		{
			global_transform_.RotateDeg(pitch, yaw, roll);
			for(auto & child : GetChildTransformables())
			{
				child->RotateDegParent(pitch, yaw, roll);
			}
		}

		// Modify just the global transform of the entity
		void ScaleParent(const float scalar)
		{
			global_transform_.Scale(scalar);
			for(auto & child : GetChildTransformables())
			{
				child->ScaleParent(scalar);
			}
		}

		// Modify just the global transform of the entity
		void ScaleParent(const glm::vec3 & multiplier)
		{
			global_transform_.Scale(multiplier);
			for(auto & child : GetChildTransformables())
			{
				child->ScaleParent(multiplier);
			}
		}

		// Modify just the global transform of the entity
		void ScaleParent(const float x, const float y, const float z)
		{
			global_transform_.Scale(x, y, z);
			for(auto & child : GetChildTransformables())
			{
				child->ScaleParent(x, y, z);
			}
		}

		// Get a list of transformable elements
		virtual const std::vector<T> & GetChildTransformables() = 0;
	};
}

