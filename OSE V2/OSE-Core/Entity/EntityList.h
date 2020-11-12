#pragma once
#include "OSE-Core/Math/Transformable.h"

namespace ose
{
	// Forward declare Entity class and EntityID typedef
	class Entity;
	typedef uint32_t EntityID;

	class EntityList : public Transformable<std::unique_ptr<Entity>>
	{
	public:
		EntityList(EntityList * parent);
		virtual ~EntityList() noexcept;
		EntityList(EntityList * parent, const EntityList & other) noexcept;
		EntityList(EntityList &&) noexcept = default;
		EntityList & operator=(EntityList &) noexcept = delete;
		EntityList & operator=(EntityList &&) noexcept = delete;

		// Add an entity to the entity list
		// TODO - should name be unique ???
		// Method constructs a new object
		// Method takes entity constructor arguments
		// Returns a reference to the newly created entity
		template<typename... Args>
		Entity * AddEntity(Args &&... params)
		{
			// construct a new entity object
			try {
				entities_.push_back(std::make_unique<Entity>(this, std::forward<Args>(params)...));
				return entities_.back().get();
			} catch(...) {
				return nullptr;
			}
		}

		// Add an entity to the entity list
		// New entity is a deep copy of the entity passed
		// Method constructs a new object
		// Returns a reference to the newly created entity
		Entity * AddEntity(const Entity & other);

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

		// Find all the entities in this entity list and sub lists with the given name
		// NOTE - If searching through all entities, use Game::FindAllEntitiesWithName instead
		std::vector<Entity *> FindDescendentEntitiesWithName(std::string_view name) const;

		// Find all the entities in this entity list and sub lists with the given name and add them to the vector passed
		void FindDescendentEntitiesWithName(std::string_view name, std::vector<Entity *> & out_vec) const;

	protected:
		// Get a list of transformable elements
		// Returns a list of child entities
		virtual const std::vector<std::unique_ptr<Entity>> & GetChildTransformables() const override { return entities_; }

		// Get a pointer to the parent transformable element
		virtual Transformable * GetParentTransformable() const override { return parent_; }

	protected:
		std::vector<std::unique_ptr<Entity>> entities_;
		EntityList * parent_ { nullptr };
	};
}

