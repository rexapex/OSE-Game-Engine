#pragma once

#include "OSE-Core/Entity/Entity.h"

namespace ose::game
{
	using namespace entity;

	class Scene
	{
	public:
		Scene(const std::string & name);
		~Scene() noexcept;
		Scene(const Scene & other) noexcept;
		Scene & operator=(const Scene & other) noexcept;

		// add a non-persistent entity to the scene
		// TODO - should name be unique ???
		// method constructs a new object
		// method takes an array of constructor arguments
		// params: name, tag, prefab
		// returns: reference to newly created entity
		template<typename... Args>
		Entity & addEntity(Args &&... params);

		// add a non-persistent entity to the scene
		// method moves the object passed
		void addEntity(std::unique_ptr<Entity> e);

		// add a non-persistent entity to the scene
		// new entity is a deep copy of the entity passed
		// method constructs a new object
		// returns: reference to newly created entity
		Entity & addEntity(const Entity & other);

		// remove entity by name
		// return true if entity with given name is removed
		// return false if no entity with given name exists
		bool removeEntity(const std::string & name);

		// remove entity by EntityID
		// return true if entity with given EntityID is removed
		// return false if no entity with given EntityID exists
		bool removeEntity(const EntityID uid);

		//get a list of all the entities in the scene (excludes persistent entities)
		const std::vector<std::unique_ptr<Entity>> & get_entities() const { return this->entities_; }

		//get the name of the scene
		const std::string & get_name() const { return this->name_; }


		//DEBUG METHODS
		//TODO - REMOVE WHEN READY
		void print();
		void printEntity(const Entity & e, int32_t level);

	private:
		std::string name_;
		std::vector<std::unique_ptr<Entity>> entities_;
	};
}

