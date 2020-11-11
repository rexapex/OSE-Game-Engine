#pragma once
#include "OSE-Core/Math/Transformable.h"
#include "OSE-Core/Entity/EntityList.h"

namespace ose
{
	class GameObject : public Transformable<std::unique_ptr<Entity>>, public EntityList
	{
	public:
		GameObject(GameObject * parent): Transformable(), EntityList(), parent_(parent) {}
		virtual ~GameObject() {}

		GameObject(GameObject * parent, GameObject const & other) noexcept
		{
			parent_ = parent;
		}

		GameObject(GameObject && other) noexcept = delete;
		GameObject & operator=(GameObject &) noexcept = delete;
		GameObject & operator=(GameObject &&) noexcept = delete;

		virtual Entity * AddEntity(std::string_view name, std::string_view tag, std::string_view prefab) override
		{
			return EntityList::AddEntity(this, name, tag, prefab);
		}

		virtual Entity * AddEntity(Entity const & other) override
		{
			return EntityList::AddEntity(this, other);
		}

		// Get a list of transformable elements
		// Returns a list of child entities
		virtual const std::vector<std::unique_ptr<Entity>> & GetChildTransformables() const override { return entities_; }

		// Get a pointer to the parent object
		// Returns nullptr if the game object has no parent
		GameObject * GetParent() const { return parent_; }

	protected:
		GameObject * parent_ { nullptr };
	};
}
