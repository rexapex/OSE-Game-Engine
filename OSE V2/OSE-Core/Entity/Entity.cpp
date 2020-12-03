#include "stdafx.h"
#include "Entity.h"
#include "OSE-Core/Game/Game.h"

namespace ose
{
	Entity::Entity(const std::string & name, const Tag tag, const std::string & prefab)
				 : Transformable(), EntityList(), ComponentList(),
					name_(name), tag_(tag), prefab_(prefab), unique_id_(Entity::NextEntityId())
	{

	}

	Entity::~Entity() noexcept
	{

	}

	Entity::Entity(EntityList * parent, Entity const & other) noexcept : EntityList(parent, other), ComponentList(other)
	{
		name_ = other.name_;
		unique_id_ = Entity::NextEntityId();
		tag_ = other.tag_;
		prefab_ = other.prefab_;

		local_transform_ = other.local_transform_;
		global_transform_ = other.global_transform_;
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
