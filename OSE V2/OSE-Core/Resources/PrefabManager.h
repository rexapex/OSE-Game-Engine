#pragma once

#include "OSE-Core/Entity/Entity.h"

namespace ose::resources
{
	using namespace entity;

	class PrefabManager
	{
	public:
		PrefabManager();
		~PrefabManager();

		// add a temporary prefab entity to the prefab manager
		// method constructs a new object
		// method takes an array of constructor arguments
	//	template<typename... Args>
	//	Entity & addTempPrefab(Args &&... params);

		// add a persistent (cached) prefab entity to the prefab manager
		// method constructs a new object
		// method takes an array of constructor arguments
	//	template<typename... Args>
	//	Entity & addCachedPrefab(Args &&... params);

	private:

		// list of all temporary entity prefabs
		// after prefab has been copied to create the new entity, the prefab object will be destroyed
		std::vector<std::unique_ptr<Entity>> temp_prefabs_;

		// list of all cached prefabs
		// these prefabs object will persist until all scenes listing the object as cached are unloaded
		std::vector<std::unique_ptr<Entity>> cached_prefabs_;
	};
}
