#pragma once
#include "OSE-Core/Types.h"
#include "Component/Component.h"
#include "EntityList.h"
#include "Component/ComponentList.h"

namespace ose
{
	typedef uint32_t EntityID;	// NOTE - Might change this to uint64_t later
	class Game;

	class Entity : public EntityList, public ComponentList
	{
	public:
		Entity(EntityList * parent, std::string const & name, std::string const & tag = "", std::string const & prefab = "");
		virtual ~Entity() noexcept;
		Entity(EntityList * parent, Entity const & other) noexcept;
		Entity(Entity && other) noexcept = default;
		Entity & operator=(Entity &) noexcept = delete;
		Entity & operator=(Entity &&) noexcept = delete;

		std::string const & GetName() const { return this->name_; }
		EntityID const GetUniqueId() const { return this->unique_id_; }

		void SetName(std::string const & name) { this->name_ = name; }
		void SetTag(std::string const & tag) { this->tag_ = tag; }

		bool IsEnabled() const { return enabled_; }
		void SetEnabled(bool a);
		void Enable();
		void Disable();

		// Should NEVER be called directly by a script
		void SetGameReference(Game * game) { game_ = game; }

	private:
		std::string name_;		// name_ need not be unique
		EntityID unique_id_;	// unique_ID_ should be unique to a game engine execution

		std::string tag_;		// the lowest level tag applied to this entity (or "")
		std::string prefab_;	// the name of the prefab this entity inherits from (or "")

		bool enabled_ { true };	// True iff the entity is enabled (i.e. it appears in the scene)

		Game * game_ { nullptr }; // Pointer to the game object this entity belongs to

		// Get the next available entity ID
		static EntityID NextEntityId()
		{
			static EntityID id { 0 };
			return id++;
		}
	};
}

