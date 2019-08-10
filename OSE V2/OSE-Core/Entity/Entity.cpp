#include "stdafx.h"
#include "Entity.h"

namespace ose::entity
{
	Entity::Entity(const std::string & name, const std::string & tag, const std::string & prefab)
				 : EntityList(), name_(name), tag_(tag), prefab_(prefab), unique_id_(Entity::NextEntityId())
	{

	}


	Entity::~Entity() noexcept
	{
		// TODO - delete components from their respective engines
		// delete all the components of this entity
		this->DeleteAllComponents();
	}


	Entity::Entity(const Entity & other) noexcept : EntityList(other)
	{
		this->name_ = other.name_;
		this->unique_id_ = Entity::NextEntityId();
		//std::cerr << this->unique_ID_ << std::endl;
		this->tag_ = other.tag_;
		this->prefab_ = other.prefab_;

		// perform a deep copy of all entities
		///this->sub_entities_ = other.sub_entities_;

		//TODO - remove any existing components
		//this->deleteAllComponents();		// NOTE - before this can be done, the components must be removed from engines
		//copy each component from other
		for(const auto & comp : other.components_)
		{
			//using a clone method prevents slicing
			this->components_.emplace_back(comp->Clone());
		}

		this->local_transform_ = other.local_transform_;
		this->global_transform_ = other.global_transform_;
	}


	//Change (Entity & other) to (Entity other) if add pointers to Entity class
	Entity & Entity::operator=(const Entity & other) noexcept
	{
		// Call the base copy assignment constructor
		EntityList::operator=(other);

		this->name_ = other.name_;
		this->unique_id_ = Entity::NextEntityId();
		//std::cerr << this->unique_ID_ << std::endl;
		this->tag_ = other.tag_;
		this->prefab_ = other.prefab_;

		// perform a deep copy of all entities
		///this->sub_entities_ = other.sub_entities_;
		
		//TODO - remove any existing components
		//this->deleteAllComponents();		// NOTE - before this can be done, the components must be removed from engines
		//copy each component from other
		for(const auto & comp : other.components_)
		{
			//using a clone method prevents slicing
			this->components_.emplace_back(comp->Clone());
		}

		this->local_transform_ = other.local_transform_;
		this->global_transform_ = other.global_transform_;

		return *this;
	}


	/*Entity::Entity(Entity && other) noexcept : EntityList(other)
	{
		this->name_ = std::move(other.name_);
		this->sub_entities_ = std::move(other.sub_entities_);
		this->unique_ID_ = std::move(other.unique_ID_);
		this->components_ = std::move(other.components_);
		this->local_transform_ = other.local_transform_;
		this->global_transform_ = other.global_transform_;
	}


	Entity & Entity::operator=(Entity && other) noexcept
	{
		// Call the base move assignment constructor
		EntityList::operator=(other);

		this->name_ = std::move(other.name_);
		this->sub_entities_ = std::move(other.sub_entities_);
		this->unique_ID_ = std::move(other.unique_ID_);
		this->components_ = std::move(other.components_);
		this->local_transform_ = other.local_transform_;
		this->global_transform_ = other.global_transform_;
		return *this;
	}*/

	// utility method for deleting all components
	void Entity::DeleteAllComponents() noexcept
	{
		// TODO - delete components from their respective engines
		this->components_.clear();
	}

	// remove the component passed from the entity
	// returns true if the component is removed
	// returns false if the component does not belong to this entity
	bool Entity::RemoveComponent(const Component & comp)
	{
		// no component can be removed if there are no components therefore return false
		if(components_.empty()) {
			return false;
		}

		// TODO - test whether this actually works
		// NOTE - remove moves removed elements to end and returns the new end as an iterator
		// NOTE - erase then deletes element between first arg and last arg from the vector
		auto size_before { components_.size() };
		components_.erase(std::remove_if(components_.begin(), components_.end(), [this, comp] (auto & component) {
			return component.get() == &comp;
		}), components_.end());
		return (size_before != components_.size());
	}
}
