#pragma once
#include "OSE-Core/Types.h"
#include "Component/Component.h"
#include "EntityList.h"
#include "Component/ComponentList.h"
#include "OSE-Core/Math/Transformable.h"

namespace ose::entity
{
	typedef uint32_t EntityID;	// NOTE - Might change this to uint64_t later

	class Entity : public EntityList, public ComponentList, public math::Transformable<std::unique_ptr<Entity>>
	{
	public:
		Entity(const std::string & name, const std::string & tag = "", const std::string & prefab = "");
		virtual ~Entity() noexcept;
		Entity(const Entity & other) noexcept;
		Entity & operator=(const Entity & other) noexcept;
		Entity(Entity && other) noexcept = default;
		Entity & operator=(Entity && other) noexcept = default;

		const std::string & GetName() const { return this->name_; }
		const EntityID GetUniqueId() const { return this->unique_id_; }

		void SetName(const std::string & name) { this->name_ = name; }
		void SetTag(const std::string & tag) { this->tag_ = tag; }


		// Get a list of transformable elements
		// Returns a list of child entities
		virtual const std::vector<std::unique_ptr<Entity>> & GetChildTransformables() override { return entities_; }

	private:

		std::string name_;		// name_ need not be unique
		EntityID unique_id_;	// unique_ID_ should be unique to a game engine execution

		std::string tag_;		// the lowest level tag applied to this entity (or "")
		std::string prefab_;	// the name of the prefab this entity inherits from (or "")

		// isVisible, isEnabled, ...

		// Get the next available entity ID
		static EntityID NextEntityId()
		{
			static EntityID id { 0 };
			return id++;
		}
	};
}

