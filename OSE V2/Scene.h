#pragma once

#include "Entity.h"

namespace origami_sheep_engine
{
	class UNIT_TEST_EXPORT Scene
	{
	public:
		Scene(const std::string & name);
		~Scene();

		std::vector<Entity> & get_entities() { return this->entities_; }

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

