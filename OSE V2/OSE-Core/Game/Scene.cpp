#include "stdafx.h"
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



	// add a non-persistent entity to the scene
	// name must be unique
	// method constructs a new object
	// method takes an array of constructor arguments
	template<typename... Args>
	Entity & Scene::addEntity(Args &&... params)
	{
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

	// remove entity by name
	// return true if entity with given name is removed
	// return false if no entity with given name exists
	bool Scene::removeEntity(const std::string & name)
	{
		// no sub entity can be removed if there are no entities, therefore return false
		if(entities_.empty()) {
			return false;
		}

		// otherwise, find the entity with the given name
		// search from beginning to end of list
		// return first component to return true from lambda
		auto & pos = std::find_if(entities_.begin(), entities_.end(), [name] (auto & entity) {
			return entity->get_name() == name;
		});

		// if a matching entity is found, remove it then return true
		if(pos != entities_.end()) {
			entities_.erase(pos);
			return true;
		}

		// else, return false
		return false;
	}

	// remove entity by EntityID
	// return true if entity with given EntityID is removed
	// return false if no entity with given EntityID exists
	bool Scene::removeEntity(const EntityID uid)
	{
		// no entity can be removed if there are no sub entities, therefore return false
		if(entities_.empty()) {
			return false;
		}

		// otherwise, find the entity with the given EntityID
		// search from beginning to end of list
		// return first component to return true from lambda
		auto & pos = std::find_if(entities_.begin(), entities_.end(), [uid] (auto & entity) {
			return entity->get_unique_ID() == uid;
		});

		// if a matching entity is found, remove it then return true
		if(pos != entities_.end()) {
			entities_.erase(pos);
			return true;
		}

		// else, return false
		return false;
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
