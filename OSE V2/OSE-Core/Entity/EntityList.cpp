#include "stdafx.h"
#include "EntityList.h"
#include "Entity.h"

namespace ose
{
	EntityList::EntityList(EntityList * parent) : Transformable(), parent_(parent)
	{
		ResetGlobalTransform();
	}

	EntityList::~EntityList() {}

	EntityList::EntityList(EntityList * parent, EntityList const & other) noexcept : Transformable(other), parent_(parent)
	{
		// Perform a deep copy of all entities
		entities_.clear();
		for(auto const & e : other.entities_)
		{
			entities_.push_back(ose::make_unique<Entity>(this, *e));
		}
		ResetGlobalTransform();
	}

	// Add an entity to the entity list
	// New entity is a deep copy of the entity passed
	// Method constructs a new object
	// Returns a reference to the newly created entity
	Entity * EntityList::AddEntity(Entity const & other)
	{
		// construct a new entity object
		try {
			entities_.push_back(ose::make_unique<Entity>(this, other));
			return entities_.back().get();
		} catch(...) {
			return nullptr;
		}
	}

	// TODO - NEEDS SERIOUS TESTING, NO IDEA WHETHER THIS WORKS
	// Remove an entity from the entity list
	// Return true if entity is removed
	// Return false if the entity does not belong to this entity list
	bool EntityList::RemoveEntity(Entity const & entity)
	{
		// no sub entity can be removed if there are no sub entities, therefore return false
		if(entities_.empty()) {
			return false;
		}

		// TODO - test whether this actually works
		// NOTE - remove moves removed elements to end and returns the new end as an iterator
		// NOTE - erase then deletes element between first arg and last arg from the vector
		size_t size_before { entities_.size() };
		entities_.erase(std::remove_if(entities_.begin(), entities_.end(), [&entity] (auto & e) {
			return e.get() == &entity;
		}), entities_.end());
		return (size_before != entities_.size());
	}

	// Remove entity by EntityID
	// Return true if entity with given EntityID is removed
	// Return false if no entity with given EntityID exists in this entity list
	bool EntityList::RemoveEntity(EntityID const uid)
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

	// Move an entity from an this entity list to a new entity list
	// Returns true if the entity is moved successfully
	// Returns false if the entity does not exist in this list
	// NOTE - Untested
	bool EntityList::MoveEntity(Entity const & entity, EntityList & to)
	{
		// No sub entity can be removed if there are no sub entities, therefore return false
		if(entities_.empty())
			return false;

		// TODO - Test whether this actually works
		// NOTE - Remove moves removed elements to end and returns the new end as an iterator
		// NOTE - Erase then deletes element between first arg and last arg from the vector
		size_t size_before { entities_.size() };
		auto iter = std::find_if(entities_.begin(), entities_.end(), [&entity] (auto & e) {
			return e.get() == &entity;
		});

		// If the entity does not exist in this entity list, return false
		if(iter == entities_.end())
			return false;

		// Swap the entity pointer into a local variable then erase the entity from this entity list
		uptr<Entity> up;
		std::swap(*iter, up);
		entities_.erase(iter);

		// Add the entity to the new entity list
		to.entities_.push_back(std::move(up));		
		return true;
	}

	// Find all the entities in this entity list and sub lists with the given name
	// NOTE - If searching through all entities, use Game::FindAllEntitiesWithName instead
	std::vector<Entity *> EntityList::FindDescendentEntitiesWithName(std::string_view name) const
	{
		std::vector<Entity *> vec;
		FindDescendentEntitiesWithName(name, vec);
		return vec;
	}

	// Find all the entities in this entity list and sub lists with the given name and add them to the vector passed
	void EntityList::FindDescendentEntitiesWithName(std::string_view name, std::vector<Entity *> & out_vec) const
	{
		for(auto const & e : entities_)
		{
			if(e->GetName() == name)
				out_vec.emplace_back(e.get());
			e->FindDescendentEntitiesWithName(name, out_vec);
		}
	}
}
