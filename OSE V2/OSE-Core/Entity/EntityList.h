#pragma once
#include "Entity.h"
#include "OSE-Core/Game/IDManager.h"

namespace ose::entity
{
	using namespace game;

	class Entity;

	class EntityList
	{
	public:
		EntityList();
		~EntityList();
		EntityList(const EntityList & other) noexcept;
		EntityList & operator=(const EntityList & other) noexcept;

		// add a non-persistent entity to the scene
		// TODO - should name be unique ???
		// method constructs a new object
		// method takes an array of constructor arguments
		// params: name, tag, prefab
		// returns: reference to newly created entity
		template<typename... Args>
		Entity & add(Args &&... params);

		// add a non-persistent entity to the scene
		// method moves the object passed
		void add(std::unique_ptr<Entity> e);

		// add a non-persistent entity to the scene
		// new entity is a deep copy of the entity passed
		// method constructs a new object
		// returns: reference to newly created entity
		Entity & add(const Entity & other);

		// TODO - NEEDS SERIOUS TESTING, NO IDEA WHETHER THIS WORKS
		// remove sub entity
		// return true if sub entity is removed
		// return false if the sub entity does not belong to this entity
		bool remove(const Entity & entity);

		// remove entity by name
		// return true if entity with given name is removed
		// return false if no entity with given name exists
		bool remove(const std::string & name);

		// remove entity by EntityID
		// return true if entity with given EntityID is removed
		// return false if no entity with given EntityID exists
		bool remove(const EntityID uid);

		//get a list of all the entities in the scene (excludes persistent entities)
		const std::vector<std::unique_ptr<Entity>> & get() const { return this->entities_; }
	private:
		std::vector<std::unique_ptr<Entity>> entities_;
	};
}

