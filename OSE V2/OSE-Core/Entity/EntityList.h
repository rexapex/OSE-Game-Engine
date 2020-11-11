#pragma once

namespace ose
{
	// Forward declare Entity class and EntityID typedef
	class Entity;
	class GameObject;
	typedef uint32_t EntityID;

	class EntityList
	{
	public:
		EntityList();
		virtual ~EntityList() noexcept;
		EntityList(const EntityList & other) noexcept;
		EntityList(EntityList &&) noexcept = default;
		EntityList & operator=(EntityList &) noexcept = delete;
		EntityList & operator=(EntityList &&) noexcept = delete;

		// Add an entity to the entity list
		virtual Entity * AddEntity(std::string_view name, std::string_view tag, std::string_view prefab) = 0;
		virtual Entity * AddEntity(Entity const & other) = 0;

		// TODO - NEEDS SERIOUS TESTING, NO IDEA WHETHER THIS WORKS
		// Remove an entity from the entity list
		// Return true if entity is removed
		// Return false if the entity does not belong to this entity list
		bool RemoveEntity(const Entity & entity);

		// Remove entity by EntityID
		// Return true if entity with given EntityID is removed
		// Return false if no entity with given EntityID exists in this entity list
		bool RemoveEntity(const EntityID uid);

		// Move an entity from an this entity list to a new entity list
		// Returns true if the entity is moved successfully
		// Returns false if the entity does not exist in this list
		bool MoveEntity(Entity const & entity, EntityList & to);

		// Get the list of entities
		const std::vector<std::unique_ptr<Entity>> & GetEntities() const { return this->entities_; }

	protected:
		// Add an entity to the entity list
		// TODO - should name be unique ???
		// Method constructs a new object
		// Method takes entity constructor arguments
		// Returns a reference to the newly created entity
		Entity * AddEntity(GameObject * parent, std::string_view name, std::string_view tag, std::string_view prefab)
		{
			// construct a new entity object
			try {
				return entities_.emplace_back( std::make_unique<Entity>(parent, name, tag, prefab) ).get();
			} catch(...) {
				return nullptr;
			}
		}

		// Add an entity to the entity list
		// New entity is a deep copy of the entity passed
		// Method constructs a new object
		// Returns a reference to the newly created entity
		Entity * AddEntity(GameObject * parent, const Entity & other);

		// The list of entities
		std::vector<std::unique_ptr<Entity>> entities_;
	};
}
