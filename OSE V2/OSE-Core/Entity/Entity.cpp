#include "stdafx.h"
#include "Entity.h"

namespace ose
{
	Entity::Entity(const std::string & name, const std::string & tag, const std::string & prefab)
				 : Transformable(), EntityList(), ComponentList(),
					name_(name), tag_(tag), prefab_(prefab), unique_id_(Entity::NextEntityId())
	{

	}


	Entity::~Entity() noexcept
	{

	}


	Entity::Entity(const Entity & other) noexcept : Transformable(other), EntityList(other), ComponentList(other)
	{
		this->name_ = other.name_;
		this->unique_id_ = Entity::NextEntityId();
		//std::cerr << this->unique_ID_ << std::endl;
		this->tag_ = other.tag_;
		this->prefab_ = other.prefab_;

		this->local_transform_ = other.local_transform_;
		this->global_transform_ = other.global_transform_;
	}


	//Change (Entity & other) to (Entity other) if add pointers to Entity class
	Entity & Entity::operator=(const Entity & other) noexcept
	{
		// Call the base copy assignment constructor
		EntityList::operator=(other);
		Transformable::operator=(other);
		ComponentList::operator=(other);

		this->name_ = other.name_;
		this->unique_id_ = Entity::NextEntityId();
		//std::cerr << this->unique_ID_ << std::endl;
		this->tag_ = other.tag_;
		this->prefab_ = other.prefab_;

		this->local_transform_ = other.local_transform_;
		this->global_transform_ = other.global_transform_;

		return *this;
	}
}
