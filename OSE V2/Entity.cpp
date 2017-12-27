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
		this->components_ = other.components_;
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
		this->components_ = other.components_;
		return *this;
	}


	Entity::Entity(Entity && other) noexcept
	{
		this->components_ = std::move(other.components_);
		this->name_ = std::move(other.name_);
		this->sub_entities_ = std::move(other.sub_entities_);
		this->unique_ID_ = std::move(other.unique_ID_);
	}


	Entity & Entity::operator=(Entity && other) noexcept
	{
		this->components_ = std::move(other.components_);
		this->name_ = std::move(other.name_);
		this->sub_entities_ = std::move(other.sub_entities_);
		this->unique_ID_ = std::move(other.unique_ID_);
		return *this;
	}
}
