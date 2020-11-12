#pragma once

#include "OSE-Core/Entity/Entity.h"

namespace ose
{
	class PrefabManager
	{
	public:
		PrefabManager();
		~PrefabManager() noexcept;

		// no copy constructor (add them in later if needed)
		PrefabManager(PrefabManager &) = delete;

		// move constructor
		PrefabManager(PrefabManager &&) noexcept;

		// No assignment constructors
		PrefabManager & operator=(PrefabManager &) = delete;
		PrefabManager & operator=(PrefabManager &&) = delete;

		// add a temporary prefab entity to the prefab manager
		// method constructs a new object
		// method takes an array of constructor arguments
		template<typename... Args>
		Entity & AddTempPrefab(Args &&... params, std::string const & path);

		// add a temporary prefab entity to the prefab manager
		// method moves the object passed
		void AddTempPrefab(uptr<Entity> e, std::string const & path);

		// add a persistent (cached) prefab entity to the prefab manager
		// method constructs a new object
		// method takes an array of constructor arguments
		template<typename... Args>
		Entity & AddCachedPrefab(Args &&... params, std::string const & path);

		// add a persistent (cached) prefab entity to the prefab manager
		// method moves the object passed
		void AddCachedPrefab(uptr<Entity> e, std::string const & path);

		// get a reference to the entity prefab with path given
		// checks both temporary and cached entities
		Entity & GetPrefab(std::string const & path);

		// returns true iff an entity prefab exists with the path given
		// check both temporary and cached entities
		bool DoesPrefabExist(std::string const & path);

		// clear the list of temporary entity prefab objects
		// frees the prefab objects from memory
		void ClearTempPrefabs();

	private:

		// map from path to temporary entity prefabs
		// after prefab has been copied to create the new entity, the prefab object will be destroyed
		std::unordered_map<std::string, uptr<Entity>> temp_prefabs_;

		// map from path to cached entity prefabs
		// these prefabs object will persist until all scenes listing the object as cached are unloaded
		std::unordered_map<std::string, uptr<Entity>> cached_prefabs_;
	};
}
