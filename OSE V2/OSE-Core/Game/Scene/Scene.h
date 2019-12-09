#pragma once

#include "OSE-Core/Entity/Entity.h"
#include "OSE-Core/Entity/EntityList.h"

namespace ose::game
{
	class Scene
	{
	public:
		Scene(const std::string & name);
		~Scene() noexcept;
		Scene(const Scene & other) noexcept;
		Scene & operator=(const Scene & other) noexcept;
		
		// provide const and non-const versions
		entity::EntityList & entities() { return entities_; };
		const entity::EntityList & entities() const { return entities_; }

		//get the name of the scene
		const std::string & get_name() const { return this->name_; }


		//DEBUG METHODS
		//TODO - REMOVE WHEN READY
		void Print();
		void PrintEntity(const entity::Entity & e, int32_t level);

	private:
		std::string name_;

		// refactored all entity add & remove operations into a separate class for code reuse
		entity::EntityList entities_;
	};
}

