#include "stdafx.h"
#include "PrefabManager.h"

namespace ose
{
	PrefabManager::PrefabManager()
	{

	}

	// Note - std::unique_ptrs will be freed automatically
	PrefabManager::~PrefabManager() noexcept {}

	// move constructors
	PrefabManager::PrefabManager(PrefabManager && o) noexcept : temp_prefabs_(std::move(o.temp_prefabs_)), cached_prefabs_(std::move(o.cached_prefabs_)) {}

	// add a temporary prefab entity to the prefab manager
	// method constructs a new object
	// method takes an array of constructor arguments
	template<typename... Args>
	Entity & PrefabManager::AddTempPrefab(Args &&... params, std::string const & path)
	{
		// construct a new entity object
		try {
			auto e = std::make_unique<Entity>(std::forward<Args>(params)...);
			auto p = temp_prefabs_.insert({ path, std::move(e) });
			return *p->second;
		} catch(std::exception & e) {
			throw e;
		}
	}

	// add a temporary prefab entity to the prefab manager
	// method moves the object passed
	void PrefabManager::AddTempPrefab(std::unique_ptr<Entity> e, std::string const & path)
	{
		if(e != nullptr) {
			try {
				// move the entity pointer to the list of entities
				temp_prefabs_.insert({ path, std::move(e) });
			} catch(std::exception & e) {
				throw e;
			}
		}
	}

	// add a persistent (cached) prefab entity to the prefab manager
	// method constructs a new object
	// method takes an array of constructor arguments
	template<typename... Args>
	Entity & PrefabManager::AddCachedPrefab(Args &&... params, std::string const & path)
	{
		// construct a new entity object
		try {
			auto e = std::make_unique<Entity>(std::forward<Args>(params)...);
			auto p = cached_prefabs_.insert({ path, std::move(e) });
			return *p->second;
		} catch(std::exception & e) {
			throw e;
		}
	}

	// add a persistent (cached) prefab entity to the prefab manager
	// method moves the object passed
	void PrefabManager::AddCachedPrefab(std::unique_ptr<Entity> e, std::string const & path)
	{
		if(e != nullptr) {
			try {
				// move the entity pointer to the list of entities
				cached_prefabs_.insert({ path, std::move(e) });
			} catch(std::exception & e) {
				throw e;
			}
		}
	}

	// get a reference to the entity prefab with name given
	// checks both temporary and cached entities
	Entity & PrefabManager::GetPrefab(std::string const & name)
	{
		// check cached prefabs
		auto iter1 = cached_prefabs_.find(name);
		if(iter1 != cached_prefabs_.end()) {
			return *iter1->second;
		}

		// check temporary prefabs
		auto iter2 = temp_prefabs_.find(name);
		if(iter2 != temp_prefabs_.end()) {
			return *iter2->second;
		}

		throw std::invalid_argument("Error: Prefab with name " + name + " does not exist");
	}

	// returns true iff an entity prefab exists with the name given
	// check both temporary and cached entities
	bool PrefabManager::DoesPrefabExist(std::string const & name)
	{
		// check cached prefabs
		auto iter1 = cached_prefabs_.find(name);
		if(iter1 != cached_prefabs_.end()) {
			return true;
		}

		// check temporary prefabs
		auto iter2 = temp_prefabs_.find(name);
		if(iter2 != temp_prefabs_.end()) {
			return true;
		}

		return false;
	}

	// clear the list of temporary entity prefab objects
	// frees the prefab objects from memory
	void PrefabManager::ClearTempPrefabs()
	{
		temp_prefabs_.clear();
	}
}
