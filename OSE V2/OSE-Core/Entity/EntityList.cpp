#include "stdafx.h"
#include "EntityList.h"
#include "Entity.h"

namespace ose
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

	// Add an entity to the entity list
	// New entity is a deep copy of the entity passed
	// Method constructs a new object
	// Returns a reference to the newly created entity
	unowned_ptr<Entity> EntityList::AddEntity(const Entity & other)
	{
		// construct a new entity object
		try {
			return entities_.emplace_back( std::make_unique<Entity>(other) ).get();
		} catch(...) {
			return nullptr;
		}
	}

	// TODO - NEEDS SERIOUS TESTING, NO IDEA WHETHER THIS WORKS
	// Remove an entity from the entity list
	// Return true if entity is removed
	// Return false if the entity does not belong to this entity list
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

	// Remove entity by EntityID
	// Return true if entity with given EntityID is removed
	// Return false if no entity with given EntityID exists in this entity list
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
