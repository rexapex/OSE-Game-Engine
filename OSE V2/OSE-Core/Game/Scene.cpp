#include "../../stdafx.h"
#include "Scene.h"

namespace ose::game
{
	Scene::Scene(const std::string & name) : name_(name) {}

	Scene::~Scene() noexcept {}

	Scene::Scene(const Scene & other) noexcept
	{
		this->name_ = other.name_;

		// perform a deep copy of all entities
		this->entities_.clear();	// vector should be empty
		for(const auto & e : other.entities_)
		{
			this->entities_.push_back(std::make_unique<Entity>(*e));
		}
	}

	Scene & Scene::operator=(const Scene & other) noexcept
	{
		this->name_ = other.name_;

		// perform a deep copy of all entities
		this->entities_.clear();	// vector should be empty
		for(const auto & e : other.entities_)
		{
			this->entities_.push_back(std::make_unique<Entity>(*e));
		}

		return *this;
	}


	//add a non-persistent entity to the scene
	//will only be added if the unique_ID_ and name_ fields are not taken
	/*void Scene::addEntity(const Entity & e)
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
	}*/


	// add a non-persistent entity to the scene
	// name must be unique
	// method constructs a new object
	// method takes an array of constructor arguments
	template<typename... Args>
	Entity & Scene::addEntity(Args &&... params)
	{
		//TODO - maybe making this a parallel for loop would be faster ????
		/*for(const auto & other : entities_)
		{
			//if the name or id is taken, then return before adding the entity
			if(other.get_name() == e.get_name() || other.get_unique_ID() == e.get_unique_ID())
			{
				return;
			}
		}*/

		//name and id not taken, therefore, add the entity to the list
		//entities_.emplace_back(e);

		// construct a new entity object
		try {
			entities_.emplace_back( std::make_unique<Entity>(std::forward<Args>(params)...) );
			return *entities_.back();	// if emplace_back succeeds, the new entity will be the last entity
		} catch(std::exception & e) {
			throw e;
		}
	}

	// add a non-persistent entity to the scene
	// method moves the object passed
	void Scene::addEntity(std::unique_ptr<Entity> e)
	{
		try {
			// move the entity pointer to the list of entities
			entities_.emplace_back( std::move(e) );
		} catch(std::exception & e) {
			throw e;
		}
	}


	// add a non-persistent entity to the scene
	// new entity is a deep copy of the entity passed
	// method constructs a new object
	// returns: reference to newly created entity
	Entity & Scene::addEntity(const Entity & other)
	{
		// construct a new entity object
		try {
			entities_.emplace_back( std::make_unique<Entity>(other) );
			return *entities_.back();	// if emplace_back succeeds, the new entity will be the last entity
		} catch(std::exception & e) {
			throw e;
		}
	}





	//DEBUG METHODS
	//TODO - REMOVE WHEN READY
	void Scene::print()
	{
		std::cerr << "**********  Scene Definition  **********" << std::endl;
		std::cerr << "scene -> name: " << this->name_ << std::endl;
		for(auto & e : entities_)
		{
			printEntity(*e, 1);
		}
	}


	void Scene::printEntity(const Entity & e, int32_t level)
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
			printEntity(*subE, level + 1);
		}
	}
}
