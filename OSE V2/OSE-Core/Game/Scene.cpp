#include "stdafx.h"
#include "Scene.h"

namespace ose::game
{
	using namespace entity;

	Scene::Scene(const std::string & name) : name_(name) {}

	Scene::~Scene() noexcept {}

	Scene::Scene(const Scene & other) noexcept
	{
		this->name_ = other.name_;
		this->entities_ = other.entities_;
	}

	Scene & Scene::operator=(const Scene & other) noexcept
	{
		this->name_ = other.name_;
		this->entities_ = other.entities_;
		return *this;
	}





	//DEBUG METHODS
	//TODO - REMOVE WHEN READY
	void Scene::Print()
	{
		std::cerr << "**********  Scene Definition  **********" << std::endl;
		std::cerr << "scene -> name: " << this->name_ << std::endl;
		for(auto & e : entities_.GetEntities())
		{
			PrintEntity(*e, 1);
		}
	}


	void Scene::PrintEntity(const Entity & e, int32_t level)
	{
		std::string indent = "";
		for(int32_t i = 0; i < level; i++)
		{
			indent += "    ";
		}

		std::cerr << indent << "entity -> id: " << e.GetUniqueId() << ", name: " << e.GetName() << std::endl;

		/*for(auto & comp : e.get_components())
		{
			std::cerr << indent << "    comp -> " << comp->name << std::endl;
		}*/

		for(const auto & subE : e.GetEntities())
		{
			PrintEntity(*subE, level + 1);
		}
	}
}
