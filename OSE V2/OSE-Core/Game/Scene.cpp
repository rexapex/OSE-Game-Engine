#include "../../stdafx.h"
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


	//add a non-persistent entity to the scene
	//will only be added if the unique_ID_ and name_ fields are not taken
	void Scene::addEntity(const Entity & e)
	{
		//TODO - maybe making this a parallel for loop would be faster ????
		for(const auto & other : entities_)
		{
			//if the name of id is taken, then return before adding the entity
			if(other.get_name() == e.get_name() || other.get_unique_ID() == e.get_unique_ID())
			{
				return;
			}
		}

		//name and id not taken, therefore, add the entity to the list
		entities_.emplace_back(e);
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

		/*for(auto & comp : e.get_components())
		{
			std::cerr << indent << "    comp -> " << comp->name << std::endl;
		}*/

		for(auto & subE : e.get_sub_entities())
		{
			printEntity(subE, level + 1);
		}
	}
}
