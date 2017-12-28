#include "stdafx.h"
#include "Entity.h"

namespace origami_sheep_engine
{
	Entity::Entity(const uint32_t unique_ID, const std::string & name, const std::string & tag = "", const std::string & prefab = "")
				 : unique_ID_(unique_ID), name_(name), tag_(tag), prefab_(prefab)
	{

	}


	Entity::~Entity() noexcept
	{
		//nothing to be freed
	}


	Entity::Entity(const Entity & other) noexcept
	{
		this->name_ = other.name_;
		this->unique_ID_ = IDManager::next_entity_ID();
		//std::cerr << this->unique_ID_ << std::endl;
		this->tag_ = other.tag_;
		this->prefab_ = other.prefab_;
		this->sub_entities_ = other.sub_entities_;	//NOTE - this does a deep copy of entity objects since they are stored by value in the vector

		//remove any existing components
		components_.clear();
		//copy each component from other
		for(const std::unique_ptr<Component> & comp : other.components_)
		{
			//using a clone method prevents slicing
			components_.push_back(comp->clone());
		}
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
		
		//remove any existing components
		components_.clear();
		//copy each component from other
		for(const std::unique_ptr<Component> & comp : other.components_)
		{
			//using a clone method prevents slicing
			components_.push_back(comp->clone());
		}

		return *this;
	}


	/*Entity::Entity(Entity && other) noexcept
	{
		this->name_ = std::move(other.name_);
		this->sub_entities_ = std::move(other.sub_entities_);
		this->unique_ID_ = std::move(other.unique_ID_);
		this->components_ = std::move(other.components_);
	}


	Entity & Entity::operator=(Entity && other) noexcept
	{
		this->name_ = std::move(other.name_);
		this->sub_entities_ = std::move(other.sub_entities_);
		this->unique_ID_ = std::move(other.unique_ID_);
		this->components_ = std::move(other.components_);
		return *this;
	}*/
}
