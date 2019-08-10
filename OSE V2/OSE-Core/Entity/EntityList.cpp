#include "stdafx.h"
#include "EntityList.h"

namespace ose::entity
{
	EntityList::EntityList() {}
	EntityList::~EntityList() {}

	EntityList::EntityList(const EntityList & other) noexcept
	{
		// perform a deep copy of all entities
		this->entities_.clear();	// vector should be empty
		for(const auto & e : other.entities_)
		{
			this->entities_.push_back(std::make_unique<Entity>(*e));
		}
	}

	EntityList & EntityList::operator=(const EntityList & other) noexcept
	{
		// perform a deep copy of all entities
		this->entities_.clear();	// vector should be empty
		for(const auto & e : other.entities_)
		{
			this->entities_.push_back(std::make_unique<Entity>(*e));
		}

		return *this;
	}



	// add a non-persistent entity to the scene
	// name must be unique
	// method constructs a new object
	// method takes an array of constructor arguments
	template<typename... Args>
	Entity & EntityList::AddEntity(Args &&... params)
	{
		// construct a new entity object
		try {
			entities_.emplace_back( std::make_unique<Entity>(std::forward<Args>(params)...) );
			return *entities_.back();	// if emplace_back succeeds, the new entity will be the last entity
		} catch(std::exception & e) {
			throw e;
		}
	}

	// add a non-persistent entity to the scene
	// method moves the object passed
	void EntityList::AddEntity(std::unique_ptr<Entity> e)
	{
		try {
			// move the entity pointer to the list of entities
			entities_.emplace_back( std::move(e) );
		} catch(std::exception & e) {
			throw e;
		}
	}


	// add a non-persistent entity to the scene
	// new entity is a deep copy of the entity passed
	// method constructs a new object
	// returns: reference to newly created entity
	Entity & EntityList::AddEntity(const Entity & other)
	{
		// construct a new entity object
		try {
			entities_.emplace_back( std::make_unique<Entity>(other) );
			return *entities_.back();	// if emplace_back succeeds, the new entity will be the last entity
		} catch(std::exception & e) {
			throw e;
		}
	}

	// remove sub entity
	// return true if sub entity is removed
	// return false if the sub entity does not belong to this entity
	bool EntityList::RemoveEntity(const Entity & entity)
	{
		// no sub entity can be removed if there are no sub entities, therefore return false
		if(entities_.empty()) {
			return false;
		}

		// TODO - test whether this actually works
		// NOTE - remove moves removed elements to end and returns the new end as an iterator
		// NOTE - erase then deletes element between first arg and last arg from the vector
		size_t size_before { entities_.size() };
		entities_.erase(std::remove_if(entities_.begin(), entities_.end(), [this, entity] (auto & e) {
			return e.get() == &entity;
		}), entities_.end());
		return (size_before != entities_.size());
	}

	// remove entity by name
	// return true if entity with given name is removed
	// return false if no entity with given name exists
	bool EntityList::RemoveEntity(const std::string & name)
	{
		// no sub entity can be removed if there are no entities, therefore return false
		if(entities_.empty()) {
			return false;
		}

		// otherwise, find the entity with the given name
		// search from beginning to end of list
		// return first component to return true from lambda
		auto & pos = std::find_if(entities_.begin(), entities_.end(), [name] (auto & entity) {
			return entity->GetName() == name;
		});

		// if a matching entity is found, remove it then return true
		if(pos != entities_.end()) {
			entities_.erase(pos);
			return true;
		}

		// else, return false
		return false;
	}

	// remove entity by EntityID
	// return true if entity with given EntityID is removed
	// return false if no entity with given EntityID exists
	bool EntityList::RemoveEntity(const EntityID uid)
	{
		// no entity can be removed if there are no sub entities, therefore return false
		if(entities_.empty()) {
			return false;
		}

		// otherwise, find the entity with the given EntityID
		// search from beginning to end of list
		// return first component to return true from lambda
		auto & pos = std::find_if(entities_.begin(), entities_.end(), [uid] (auto & entity) {
			return entity->GetUniqueId() == uid;
		});

		// if a matching entity is found, remove it then return true
		if(pos != entities_.end()) {
			entities_.erase(pos);
			return true;
		}

		// else, return false
		return false;
	}


}
