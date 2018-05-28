#pragma once

#include "OSE-Core/Entity/Entity.h"

namespace ose::resources
{
	using namespace entity;

	class PrefabManager
	{
	public:
		PrefabManager();
		~PrefabManager() noexcept;

		// no copy constructors (add them in later if needed)
		PrefabManager(PrefabManager &) = delete;
		PrefabManager & operator=(PrefabManager &) = delete;

		// move constructors
		PrefabManager(PrefabManager &&) noexcept;
		PrefabManager & operator=(PrefabManager &&) noexcept;

		// add a temporary prefab entity to the prefab manager
		// method constructs a new object
		// method takes an array of constructor arguments
		template<typename... Args>
		Entity & addTempPrefab(Args &&... params, std::string const & path);

		// add a temporary prefab entity to the prefab manager
		// method moves the object passed
		void addTempPrefab(std::unique_ptr<Entity> e, std::string const & path);

		// add a persistent (cached) prefab entity to the prefab manager
		// method constructs a new object
		// method takes an array of constructor arguments
		template<typename... Args>
		Entity & addCachedPrefab(Args &&... params, std::string const & path);

		// add a persistent (cached) prefab entity to the prefab manager
		// method moves the object passed
		void addCachedPrefab(std::unique_ptr<Entity> e, std::string const & path);

		// get a reference to the entity prefab with path given
		// checks both temporary and cached entities
		Entity & getPrefab(std::string const & path);

		// returns true iff an entity prefab exists with the path given
		// check both temporary and cached entities
		bool doesPrefabExist(std::string const & path);

		// clear the list of temporary entity prefab objects
		// frees the prefab objects from memory
		void clearTempPrefabs();

	private:

		// map from path to temporary entity prefabs
		// after prefab has been copied to create the new entity, the prefab object will be destroyed
		std::unordered_map<std::string, std::unique_ptr<Entity>> temp_prefabs_;

		// map from path to cached entity prefabs
		// these prefabs object will persist until all scenes listing the object as cached are unloaded
		std::unordered_map<std::string, std::unique_ptr<Entity>> cached_prefabs_;
	};
}
