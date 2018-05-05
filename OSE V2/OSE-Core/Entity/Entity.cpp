#include "../../stdafx.h"
#include "Entity.h"

namespace ose::entity
{
	Entity::Entity(const std::string & name, const std::string & tag = "", const std::string & prefab = "")
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
		this->sub_entities_ = other.sub_entities_;	//NOTE - this does a deep copy of entity objects since they are stored by value in the vector

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
		// delete all the components of this entity
		/*for(Component * comp : this->components_)
		{
			delete comp;
		}*/
		this->components_.clear();
	}

	/*
		Sub-Entity manipulation methods
	*/

	// add a sub entity to the entity
	// method constructs a new object
	// method takes an array of constructor arguments
	template<typename... Args>
	void Entity::addSubEntity(Args &&... params)
	{
		sub_entities_.emplace_back( std::make_unique<Entity>(std::forward<Args>(params)...) );
	}

	/**
		Entity component manipulation methods
		Add & Remove components by component type
	*/

	//***************
	// Entity::addComponent
	// perfect-forwards all params to the ComponentType constructor with the matching parameter list
	// DEBUG: be sure to compare the arguments of this fn to the desired constructor to avoid perfect-forwarding failure cases
	// EG: deduced initializer lists, decl-only static const int members, 0|NULL instead of nullptr, overloaded fn names, and bitfields
	//***************
	template<class ComponentType, typename... Args>
	void Entity::addComponent(Args &&... params)
	{
		//components.emplace_back(new ComponentType(std::forward<Args>(params)));
		components.emplace_back( std::make_unique<ComponentType>(std::forward<Args>(params)...) );
	}

	//***************
	// Entity::getComponent
	// returns the first component that matches the template type
	// or that is derived from the template type
	// EG: if the template type is Component, and components[0] type is BoxCollider
	// then components[0] will be returned because it derives from Component
	//***************
	template<class ComponentType>
	ComponentType * Entity::getComponent() const
	{
		// check whether the type matches of each component
		for(auto && component : components)
		{
			// if the type is correct, return a pointer to the component
			if(component->isClassType(ComponentType::Type)) {
				return static_cast<ComponentType *>(component->get());
			}
		}

		return nullptr;	// returns nullptr if no component of type given exists
	}


	/**
		Entity transformation methods
		Updates both the local and global transforms
	*/

	void Entity::translate(const glm::vec3 & translation)
	{
		local_transform_.translate(translation);
		global_transform_.translate(translation);
		for(auto & sub_entity : sub_entities_)
		{
			sub_entity->translateParent(translation);
		}
	}

	void Entity::translate(const float x, const float y, const float z)
	{
		local_transform_.translate(x, y, z);
		global_transform_.translate(x, y, z);
		for(auto & sub_entity : sub_entities_)
		{
			sub_entity->translateParent(x, y, z);
		}
	}

	//rotate by radians
	void Entity::rotate(const glm::vec3 & change)
	{
		local_transform_.rotate(change);
		global_transform_.rotate(change);
		for(auto & sub_entity : sub_entities_)
		{
			sub_entity->rotateParent(change);
		}
	}

	void Entity::rotate(const float pitch, const float yaw, const float roll)
	{
		local_transform_.rotate(pitch, yaw, roll);
		global_transform_.rotate(pitch, yaw, roll);
		for(auto & sub_entity : sub_entities_)
		{
			sub_entity->rotateParent(pitch, yaw, roll);
		}
	}

	//rotate by degrees
	void Entity::rotateDeg(const glm::vec3 & change)
	{
		local_transform_.rotateDeg(change);
		global_transform_.rotateDeg(change);
		for(auto & sub_entity : sub_entities_)
		{
			sub_entity->rotateDegParent(change);
		}
	}

	void Entity::rotateDeg(const float pitch, const float yaw, const float roll)
	{
		local_transform_.rotateDeg(pitch, yaw, roll);
		global_transform_.rotateDeg(pitch, yaw, roll);
		for(auto & sub_entity : sub_entities_)
		{
			sub_entity->rotateDegParent(pitch, yaw, roll);
		}
	}

	void Entity::scale(const float scalar)
	{
		local_transform_.scale(scalar);
		global_transform_.scale(scalar);
		for(auto & sub_entity : sub_entities_)
		{
			sub_entity->scaleParent(scalar);
		}
	}

	void Entity::scale(const glm::vec3 & multiplier)
	{
		local_transform_.scale(multiplier);
		global_transform_.scale(multiplier);
		for(auto & sub_entity : sub_entities_)
		{
			sub_entity->scaleParent(multiplier);
		}
	}

	void Entity::scale(const float x, const float y, const float z)
	{
		local_transform_.scale(x, y, z);
		global_transform_.scale(x, y, z);
		for(auto & sub_entity : sub_entities_)
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
		for(auto & sub_entity : sub_entities_)
		{
			sub_entity->translateParent(translation);
		}
	}

	void Entity::translateParent(const float x, const float y, const float z)
	{
		global_transform_.translate(x, y, z);
		for(auto & sub_entity : sub_entities_)
		{
			sub_entity->translateParent(x, y, z);
		}
	}

	//rotate by radians
	void Entity::rotateParent(const glm::vec3 & change)
	{
		global_transform_.rotate(change);
		for(auto & sub_entity : sub_entities_)
		{
			sub_entity->rotateParent(change);
		}
	}

	void Entity::rotateParent(const float pitch, const float yaw, const float roll)
	{
		global_transform_.rotate(pitch, yaw, roll);
		for(auto & sub_entity : sub_entities_)
		{
			sub_entity->rotateParent(pitch, yaw, roll);
		}
	}

	//rotate by degrees
	void Entity::rotateDegParent(const glm::vec3 & change)
	{
		global_transform_.rotateDeg(change);
		for(auto & sub_entity : sub_entities_)
		{
			sub_entity->rotateDegParent(change);
		}
	}

	void Entity::rotateDegParent(const float pitch, const float yaw, const float roll)
	{
		global_transform_.rotateDeg(pitch, yaw, roll);
		for(auto & sub_entity : sub_entities_)
		{
			sub_entity->rotateDegParent(pitch, yaw, roll);
		}
	}

	void Entity::scaleParent(const float scalar)
	{
		global_transform_.scale(scalar);
		for(auto & sub_entity : sub_entities_)
		{
			sub_entity->scaleParent(scalar);
		}
	}

	void Entity::scaleParent(const glm::vec3 & multiplier)
	{
		global_transform_.scale(multiplier);
		for(auto & sub_entity : sub_entities_)
		{
			sub_entity->scaleParent(multiplier);
		}
	}

	void Entity::scaleParent(const float x, const float y, const float z)
	{
		global_transform_.scale(x, y, z);
		for(auto & sub_entity : sub_entities_)
		{
			sub_entity->scaleParent(x, y, z);
		}
	}
}
