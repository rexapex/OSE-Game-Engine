#include "stdafx.h"
#include "Entity.h"

namespace ose::entity
{
	using namespace math;

	Entity::Entity(const std::string & name, const std::string & tag, const std::string & prefab)
				 : Transformable(), EntityList(), name_(name), tag_(tag), prefab_(prefab), unique_id_(Entity::NextEntityId())
	{

	}


	Entity::~Entity() noexcept
	{
		// TODO - delete components from their respective engines
		// delete all the components of this entity
		this->DeleteAllComponents();
	}


	Entity::Entity(const Entity & other) noexcept : Transformable(other), EntityList(other)
	{
		this->name_ = other.name_;
		this->unique_id_ = Entity::NextEntityId();
		//std::cerr << this->unique_ID_ << std::endl;
		this->tag_ = other.tag_;
		this->prefab_ = other.prefab_;

		// perform a deep copy of all entities
		///this->sub_entities_ = other.sub_entities_;

		//TODO - remove any existing components
		this->DeleteAllComponents();		// NOTE - before this can be done, the components must be removed from engines
		//copy each component from other
		for(const auto & comp : other.components_)
		{
			// Component base class won't compile if abstract so check for it here instead (and elsewhere)
			if(comp->GetClassType() != Component::GetClassType()) {
				//using a clone method prevents slicing
				this->components_.emplace_back(comp->Clone());
			}
		}

		this->local_transform_ = other.local_transform_;
		this->global_transform_ = other.global_transform_;
	}


	//Change (Entity & other) to (Entity other) if add pointers to Entity class
	Entity & Entity::operator=(const Entity & other) noexcept
	{
		// Call the base copy assignment constructor
		EntityList::operator=(other);
		Transformable::operator=(other);

		this->name_ = other.name_;
		this->unique_id_ = Entity::NextEntityId();
		//std::cerr << this->unique_ID_ << std::endl;
		this->tag_ = other.tag_;
		this->prefab_ = other.prefab_;

		// perform a deep copy of all entities
		///this->sub_entities_ = other.sub_entities_;
		
		//TODO - remove any existing components
		this->DeleteAllComponents();		// NOTE - before this can be done, the components must be removed from engines
		//copy each component from other
		for(const auto & comp : other.components_)
		{
			// Component base class won't compile if abstract so check for it here instead (and elsewhere)
			if(comp->GetClassType() != Component::GetClassType()) {
				//using a clone method prevents slicing
				this->components_.emplace_back(comp->Clone());
			}
		}

		this->local_transform_ = other.local_transform_;
		this->global_transform_ = other.global_transform_;

		return *this;
	}
	
	// utility method for deleting all components
	void Entity::DeleteAllComponents() noexcept
	{
		// TODO - delete components from their respective engines
		this->components_.clear();
	}

	// remove the component passed from the entity
	// does NOT delete the component
	// returns true if the component is removed
	// returns false if the component does not belong to this entity
	bool Entity::RemoveComponent(const ose::unowned_ptr<Component> comp)
	{
		// No component can be removed if there are no components therefore return false
		// Nullptr cannot be in list of components_ therefore exit if comp == nullptr
		if(components_.empty() || comp == nullptr) {
			return false;
		}

		// TODO - test whether this actually works
		// NOTE - remove moves removed elements to end and returns the new end as an iterator
		// NOTE - erase then deletes element between first arg and last arg from the vector
		size_t size_before { components_.size() };
		components_.erase(std::remove_if(components_.begin(), components_.end(), [this, comp] (auto & component) {
			return component.get() == comp;
		}), components_.end());
		return (size_before != components_.size());
	}
}
