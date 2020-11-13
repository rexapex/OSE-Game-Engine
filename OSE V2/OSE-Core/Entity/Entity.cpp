#include "stdafx.h"
#include "Entity.h"
#include "OSE-Core/Game/Game.h"

namespace ose
{
	Entity::Entity(EntityList * parent, std::string const & name, std::string const & tag, std::string const & prefab)
				 : EntityList(parent), ComponentList(),
					name_(name), tag_(tag), prefab_(prefab), unique_id_(Entity::NextEntityId())
	{

	}

	Entity::~Entity() noexcept
	{

	}

	Entity::Entity(EntityList * parent, Entity const & other) noexcept : EntityList(parent, other), ComponentList(other)
	{
		this->name_ = other.name_;
		this->unique_id_ = Entity::NextEntityId();
		this->tag_ = other.tag_;
		this->prefab_ = other.prefab_;

		this->local_transform_ = other.local_transform_;
		this->global_transform_ = other.global_transform_;
	}

	void Entity::SetEnabled(bool a)
	{
		enabled_ = a;
		if(game_ && a)
			game_->OnEntityActivated(*this);
		else if(game_ && !a)
			game_->OnEntityDeactivated(*this);
	}

	void Entity::Enable()
	{
		enabled_ = true;
		if(game_)
			game_->OnEntityActivated(*this);
	}

	void Entity::Disable()
	{
		enabled_ = false;
		if(game_)
			game_->OnEntityDeactivated(*this);
	}
}
