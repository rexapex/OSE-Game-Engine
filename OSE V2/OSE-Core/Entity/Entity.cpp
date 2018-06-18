#include "stdafx.h"
#include "Entity.h"

namespace ose::entity
{
	Entity::Entity(const std::string & name, const std::string & tag, const std::string & prefab)
				 : name_(name), tag_(tag), prefab_(prefab)
	{
		this->unique_ID_ = IDManager::next_entity_ID();
	}


	Entity::~Entity() noexcept
	{
		// TODO - delete components from their respective engines
		// delete all the components of this entity
		this->deleteAllComponents();
	}


	Entity::Entity(const Entity & other) noexcept
	{
		this->name_ = other.name_;
		this->unique_ID_ = IDManager::next_entity_ID();
		//std::cerr << this->unique_ID_ << std::endl;
		this->tag_ = other.tag_;
		this->prefab_ = other.prefab_;

		// perform a deep copy of all entities
		this->sub_entities_ = other.sub_entities_;

		//TODO - remove any existing components
		//this->deleteAllComponents();		// NOTE - before this can be done, the components must be removed from engines
		//copy each component from other
		for(const auto & comp : other.components_)
		{
			//using a clone method prevents slicing
			this->components_.emplace_back(comp->clone());
		}

		this->local_transform_ = other.local_transform_;
		this->global_transform_ = other.global_transform_;
	}


	//Change (Entity & other) to (Entity other) if add pointers to Entity class
	Entity & Entity::operator=(const Entity & other) noexcept
	{
		this->name_ = other.name_;
		this->unique_ID_ = IDManager::next_entity_ID();
		//std::cerr << this->unique_ID_ << std::endl;
		this->tag_ = other.tag_;
		this->prefab_ = other.prefab_;

		// perform a deep copy of all entities
		this->sub_entities_ = other.sub_entities_;
		
		//TODO - remove any existing components
		//this->deleteAllComponents();		// NOTE - before this can be done, the components must be removed from engines
		//copy each component from other
		for(const auto & comp : other.components_)
		{
			//using a clone method prevents slicing
			this->components_.emplace_back(comp->clone());
		}

		this->local_transform_ = other.local_transform_;
		this->global_transform_ = other.global_transform_;

		return *this;
	}


	/*Entity::Entity(Entity && other) noexcept
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
		this->name_ = std::move(other.name_);
		this->sub_entities_ = std::move(other.sub_entities_);
		this->unique_ID_ = std::move(other.unique_ID_);
		this->components_ = std::move(other.components_);
		this->local_transform_ = other.local_transform_;
		this->global_transform_ = other.global_transform_;
		return *this;
	}*/

	// utility method for deleting all components
	void Entity::deleteAllComponents() noexcept
	{
		// TODO - delete components from their respective engines
		this->components_.clear();
	}


	/**
		Entity component manipulation methods
		Add & Remove components by component type
	*/

	// add a component to the entity by component type
	// method constructs a new object of the given component type
	// template takes the type of component
	// method takes an array of contructor arguments
	/*template<class ComponentType, typename... Args>
	void Entity::addComponent(Args &&... params)
	{
		//components.emplace_back(new ComponentType(std::forward<Args>(params)));
		components.emplace_back( std::make_unique<ComponentType>(std::forward<Args>(params)...) );
	}*/

	// get the first component of specified type
	// returns reference to component if one exists
	// throws std::invalid_argument if component of given type does not exist
	// entity class manages pointer, returned pointer should not be deleted (de-allocated)
	// returns nullptr if no component of type given exists
	/*template<class ComponentType>
	ComponentType & Entity::getComponent() const
	{
		// check whether the type matches of each component
		for(auto && component : components)
		{
			// if the type is correct, return a pointer to the component
			if(component->isClassType(ComponentType::Type)) {
				return static_cast<ComponentType>(*component);
			}
		}

		throw std::invalid_argument("Error: Component does not exist");	// returns nullptr if no component of type given exists
	}*/

	// get a list of components of specified type
	// returns list of references
	// list will be empty if no component of given type exists
	/*template<class ComponentType>
	std::vector<ComponentType *> Entity::getComponents() const
	{
		std::vector<ComponentType *> matching_comps;

		for(auto && comp : components_)
		{
			// add every component which is/derives from the type given
			if(comp->isClassType(ComponentType::Type)) {
				matching_comps.emplace_back(static_cast<ComponentType*>(comp.get()));
			}
		}
		
		return matching_comps;
	}*/

	// remove the first component of specified type
	// returns true if component of given type is removed
	// returns false if no component of given type exists
	/*template<class ComponentType>
	bool Entity::removeComponent()
	{
		// no component can be removed if there are no components therefore return false
		if(components_.empty()) {
			return false;
		}

		// otherwise, find the first component of given type
		// search from beginning to end of list
		// return first component to return true from lambda
		auto & pos = std::find_if(components_.begin(), components_.end(), [type = ComponentType::Type] (auto & comp) {
			return comp->isClassType(type);
		});

		// if a matching component is found, remove it then return true
		if(pos != components_.end()) {
			components_.erase(pos);
			return true;
		}

		// else, return false
		return false;
	}*/

	// remove the component pass from the entity
	// returns true if the component is removed
	// returns false if the component does not belong to this entity
	bool Entity::removeComponent(const Component & comp)
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

	// remove all components which are of / are derived from given type
	// returns the number of removals
	/*template<class ComponentType>
	int32_t Entity::removeComponents()
	{
		// no component can be removed if there are no components therefore return 0
		if(components_.empty()) {
			return 0;
		}

		int32_t num_removals = 0;

		// use removeComponent method in a loop until no more components can be removed
		do {
			bool removed = this->removeComponent<ComponentType>();
			if(removed) num_removals ++;
		} while(removed);

		return num_removals;
	}*/


	/**
		Entity transformation methods
		Updates both the local and global transforms
	*/

	void Entity::translate(const glm::vec3 & translation)
	{
		local_transform_.translate(translation);
		global_transform_.translate(translation);
		for(auto & sub_entity : sub_entities_.get())
		{
			sub_entity->translateParent(translation);
		}
	}

	void Entity::translate(const float x, const float y, const float z)
	{
		local_transform_.translate(x, y, z);
		global_transform_.translate(x, y, z);
		for(auto & sub_entity : sub_entities_.get())
		{
			sub_entity->translateParent(x, y, z);
		}
	}

	//rotate by radians
	void Entity::rotate(const glm::vec3 & change)
	{
		local_transform_.rotate(change);
		global_transform_.rotate(change);
		for(auto & sub_entity : sub_entities_.get())
		{
			sub_entity->rotateParent(change);
		}
	}

	void Entity::rotate(const float pitch, const float yaw, const float roll)
	{
		local_transform_.rotate(pitch, yaw, roll);
		global_transform_.rotate(pitch, yaw, roll);
		for(auto & sub_entity : sub_entities_.get())
		{
			sub_entity->rotateParent(pitch, yaw, roll);
		}
	}

	//rotate by degrees
	void Entity::rotateDeg(const glm::vec3 & change)
	{
		local_transform_.rotateDeg(change);
		global_transform_.rotateDeg(change);
		for(auto & sub_entity : sub_entities_.get())
		{
			sub_entity->rotateDegParent(change);
		}
	}

	void Entity::rotateDeg(const float pitch, const float yaw, const float roll)
	{
		local_transform_.rotateDeg(pitch, yaw, roll);
		global_transform_.rotateDeg(pitch, yaw, roll);
		for(auto & sub_entity : sub_entities_.get())
		{
			sub_entity->rotateDegParent(pitch, yaw, roll);
		}
	}

	void Entity::scale(const float scalar)
	{
		local_transform_.scale(scalar);
		global_transform_.scale(scalar);
		for(auto & sub_entity : sub_entities_.get())
		{
			sub_entity->scaleParent(scalar);
		}
	}

	void Entity::scale(const glm::vec3 & multiplier)
	{
		local_transform_.scale(multiplier);
		global_transform_.scale(multiplier);
		for(auto & sub_entity : sub_entities_.get())
		{
			sub_entity->scaleParent(multiplier);
		}
	}

	void Entity::scale(const float x, const float y, const float z)
	{
		local_transform_.scale(x, y, z);
		global_transform_.scale(x, y, z);
		for(auto & sub_entity : sub_entities_.get())
		{
			sub_entity->scaleParent(x, y, z);
		}
	}



	/**
	Entity transformation methods
	Updates just the global transform, i.e. for when parent entity has been transformed
	*/

	void Entity::translateParent(const glm::vec3 & translation)
	{
		global_transform_.translate(translation);
		for(auto & sub_entity : sub_entities_.get())
		{
			sub_entity->translateParent(translation);
		}
	}

	void Entity::translateParent(const float x, const float y, const float z)
	{
		global_transform_.translate(x, y, z);
		for(auto & sub_entity : sub_entities_.get())
		{
			sub_entity->translateParent(x, y, z);
		}
	}

	//rotate by radians
	void Entity::rotateParent(const glm::vec3 & change)
	{
		global_transform_.rotate(change);
		for(auto & sub_entity : sub_entities_.get())
		{
			sub_entity->rotateParent(change);
		}
	}

	void Entity::rotateParent(const float pitch, const float yaw, const float roll)
	{
		global_transform_.rotate(pitch, yaw, roll);
		for(auto & sub_entity : sub_entities_.get())
		{
			sub_entity->rotateParent(pitch, yaw, roll);
		}
	}

	//rotate by degrees
	void Entity::rotateDegParent(const glm::vec3 & change)
	{
		global_transform_.rotateDeg(change);
		for(auto & sub_entity : sub_entities_.get())
		{
			sub_entity->rotateDegParent(change);
		}
	}

	void Entity::rotateDegParent(const float pitch, const float yaw, const float roll)
	{
		global_transform_.rotateDeg(pitch, yaw, roll);
		for(auto & sub_entity : sub_entities_.get())
		{
			sub_entity->rotateDegParent(pitch, yaw, roll);
		}
	}

	void Entity::scaleParent(const float scalar)
	{
		global_transform_.scale(scalar);
		for(auto & sub_entity : sub_entities_.get())
		{
			sub_entity->scaleParent(scalar);
		}
	}

	void Entity::scaleParent(const glm::vec3 & multiplier)
	{
		global_transform_.scale(multiplier);
		for(auto & sub_entity : sub_entities_.get())
		{
			sub_entity->scaleParent(multiplier);
		}
	}

	void Entity::scaleParent(const float x, const float y, const float z)
	{
		global_transform_.scale(x, y, z);
		for(auto & sub_entity : sub_entities_.get())
		{
			sub_entity->scaleParent(x, y, z);
		}
	}
}
