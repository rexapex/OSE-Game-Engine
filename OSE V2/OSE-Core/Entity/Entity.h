#pragma once
#include "OSE-Core/Types.h"
#include "Component/Component.h"
#include "EntityList.h"
#include "Component/ComponentList.h"
#include "OSE-Core/Game/Tagging/Tag.h"

namespace ose
{
	typedef uint32_t EntityID;	// NOTE - Might change this to uint64_t later
	class Game;

	class Entity : public EntityList, public ComponentList
	{
	public:
		Entity(std::string const & name, Tag const tag, std::string const & prefab = "");
		virtual ~Entity() noexcept;
		Entity(EntityList * parent, Entity const & other) noexcept;
		Entity(Entity && other) noexcept = default;
		Entity & operator=(Entity &) noexcept = delete;
		Entity & operator=(Entity &&) noexcept = delete;

		std::string const & GetName() const { return name_; }
		EntityID const GetUniqueId() const { return unique_id_; }
		Tag GetTag() const { return tag_; }

		void SetName(std::string const & name) { name_ = name; }
		void SetTag(Tag const tag) { tag_ = tag; }

		bool IsEnabled() const { return enabled_; }
		void SetEnabled(bool a);
		void Enable();
		void Disable();

		// Should NEVER be called directly by a script
		void SetGameReference(Game * game) { game_ = game; }

	private:
		std::string name_;		// name_ need not be unique
		EntityID unique_id_;	// unique_ID_ should be unique to a game engine execution

		Tag tag_;				// the lowest level tag applied to this entity
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

