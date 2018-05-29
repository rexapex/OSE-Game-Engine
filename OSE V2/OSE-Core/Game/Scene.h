#pragma once

#include "OSE-Core/Entity/Entity.h"
#include "OSE-Core/Entity/EntityList.h"

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
		
		// provide const and non-const versions
		EntityList & entities() { return entities_; };
		const EntityList & entities() const { return entities_; }

		//get the name of the scene
		const std::string & get_name() const { return this->name_; }


		//DEBUG METHODS
		//TODO - REMOVE WHEN READY
		void print();
		void printEntity(const Entity & e, int32_t level);

	private:
		std::string name_;

		// refactored all entity add & remove operations into a separate class for code reuse
		EntityList entities_;
	};
}

