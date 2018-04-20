#pragma once

#include "../Entity/Entity.h"

namespace ose::game
{
	using namespace entity;

	class Scene
	{
	public:
		Scene(const std::string & name);
		~Scene() noexcept;
		Scene(const Scene & other) noexcept;
		Scene & operator=(const Scene & other) noexcept;

		//add a non-persistent entity to the scene
		//will only be added if the unique_ID_ and name_ fields are not taken
		void addEntity(const Entity & e);

		//get a list of all the entities in the scene (excludes persistent entities)
		std::vector<Entity> & get_entities() { return this->entities_; }

		//get the name of the scene
		const std::string & get_name() const { return this->name_; }


		//DEBUG METHODS
		//TODO - REMOVE WHEN READY
		void print();
		void printEntity(Entity & e, int32_t level);

	private:
		std::string name_;
		std::vector<Entity> entities_;
	};
}

