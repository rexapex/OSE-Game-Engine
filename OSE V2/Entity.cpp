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

	}


	Entity::Entity(Entity && other) noexcept
	{
		this->mesh_ = std::move(other.mesh_);
		this->mesh_renderer_ = std::move(other.mesh_renderer_);
		this->name_ = std::move(other.name_);
		this->sub_entities_ = std::move(other.sub_entities_);
		this->unique_ID_ = std::move(other.unique_ID_);
	}


	Entity & Entity::operator=(Entity && other) noexcept
	{
		this->mesh_ = std::move(other.mesh_);
		this->mesh_renderer_ = std::move(other.mesh_renderer_);
		this->name_ = std::move(other.name_);
		this->sub_entities_ = std::move(other.sub_entities_);
		this->unique_ID_ = std::move(other.unique_ID_);
		return *this;
	}
}
