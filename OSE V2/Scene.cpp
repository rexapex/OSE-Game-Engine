#include "stdafx.h"
#include "Scene.h"

namespace origami_sheep_engine
{
	Scene::Scene(const std::string & name) : name_(name) {}

	Scene::~Scene() noexcept {}

	Scene::Scene(const Scene & other) noexcept
	{
		this->name_ = other.name_;
		this->entities_ = other.entities_;	//NOTE - this does a deep copy of entity objects since they are stored by value in the vector
	}

	Scene & Scene::operator=(const Scene & other) noexcept
	{
		this->name_ = other.name_;
		this->entities_ = other.entities_;	//NOTE - this does a deep copy of entity objects since they are stored by value in the vector
		return *this;
	}







	//DEBUG METHODS
	//TODO - REMOVE WHEN READY
	void Scene::print()
	{
		std::cerr << "**********  Scene Definition  **********" << std::endl;
		std::cerr << "scene -> name: " << this->name_ << std::endl;
		for(auto & e : entities_)
		{
			printEntity(e, 1);
		}
	}


	void Scene::printEntity(Entity & e, int32_t level)
	{
		std::string indent = "";
		for(int32_t i = 0; i < level; i++)
		{
			indent += "    ";
		}

		std::cerr << indent << "entity -> id: " << e.get_unique_ID() << ", name: " << e.get_name() << std::endl;

		if(e.mesh_)
			std::cerr << indent << "    " << "mesh -> name: " << e.mesh_->name << std::endl;

		for(auto & subE : e.get_sub_entities())
		{
			printEntity(subE, level + 1);
		}
	}
}
