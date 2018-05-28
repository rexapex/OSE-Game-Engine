#pragma once
#include "Component.h"
#include "OSE-Core/Game/IDManager.h"
#include "OSE-Core/Math/Transform.h"
#include "OSE-Core/Game/IDManager.h"

namespace ose::entity
{
	using namespace math;
	using namespace game;

	class Entity
	{
	public:
		Entity(const std::string & name, const std::string & tag, const std::string & prefab);
		virtual ~Entity() noexcept;
		Entity(const Entity & other) noexcept;
		Entity & operator=(const Entity & other) noexcept;
		Entity(Entity && other) noexcept = default;
		Entity & operator=(Entity && other) noexcept = default;

		const std::string & get_name() const { return this->name_; }
		const EntityID get_unique_ID() const { return this->unique_ID_; }

		void set_name(const std::string & name) { this->name_ = name; }
		void set_tag(const std::string & tag) { this->tag_ = tag; }

		// get a list of all sub entities
		const std::vector<std::unique_ptr<Entity>> & get_sub_entities() const { return this->sub_entities_; }

		// get a list of all components
		const std::vector<std::unique_ptr<Component>> & get_components() const { return this->components_; }

		// add a sub entity to the entity
		// method constructs a new object
		// method takes an array of constructor arguments
		// returns: reference to newly created entity
		template<typename... Args>
		Entity & addSubEntity(Args &&... params);

		// add a sub entity to the entity
		// method moves the object passed
		void addSubEntity(std::unique_ptr<Entity> e);

		// add a sub entity to the entity
		// new entity is a deep copy of the entity passed
		// method constructs a new object
		// returns: reference to newly created entity
		Entity & addSubEntity(const Entity & other);

		// TODO - NEEDS SERIOUS TESTING, NO IDEA WHETHER THIS WORKS
		// remove sub entity
		// return true if sub entity is removed
		// return false if the sub entity does not belong to this entity
		bool removeSubEntity(const Entity & entity);

		// remove sub entity by name
		// return true if sub entity with given name is removed
		// return false if no sub entity with given name exists
		bool removeSubEntity(const std::string & name);

		// remove sub entity by EntityID
		// return true if sub entity with given EntityID is removed
		// return false if no sub entity with given EntityID exists
		bool removeSubEntity(const EntityID uid);

		// add a component to the entity by component type
		// method constructs a new object of the given component type
		// template takes the type of component
		// method takes an array of contructor arguments
		template<class ComponentType, typename... Args>
		void addComponent(Args &&... params);

		// get the first component of specified type
		// returns reference to component if one exists
		// throws std::invalid_argument if component of given type does not exist
		// entity class manages object, returned object should not be deleted (de-allocated)
		template<class ComponentType>
		ComponentType & getComponent() const;

		// get a list of components of specified type
		// returns list of references
		// list will be empty if no component of given type exists
		template<class ComponentType>
		std::vector<ComponentType &> getComponents() const;

		// remove the first component of specified type
		// returns true if component of given type is removed
		// returns false if no component of given type exists
		template<class ComponentType>
		bool removeComponent();

		// TODO - NEEDS SERIOUS TESTING, NO IDEA WHETHER THIS WORKS
		// remove the component pass from the entity
		// returns true if the component is removed
		// returns false if the component does not belong to this entity
		bool removeComponent(const Component & comp);

		// remove all components which are of / are derived from given type
		// returns the number of removals
		template<class ComponentType>
		int32_t removeComponents();

		// read-only transform relative the parent entity/world if no parent exists
		const Transform & get_local_transform() const { return this->local_transform_; }

		// read-only transform relative to the world
		const Transform & get_global_transform() const { return this->global_transform_; }

		// modify the local and global transform of the entity
		void translate(const glm::vec3 & translation);
		void translate(const float x, const float y, const float z);

		// rotate by radians
		void rotate(const glm::vec3 & change);
		void rotate(const float pitch, const float yaw, const float roll);
		// rotate by degrees
		void rotateDeg(const glm::vec3 & change);
		void rotateDeg(const float pitch, const float yaw, const float roll);

		void scale(const float scalar);
		void scale(const glm::vec3 & multiplier);
		void scale(const float x, const float y, const float z);

	private:
		// modify just the global transform of the entity
		void translateParent(const glm::vec3 & translation);
		void translateParent(const float x, const float y, const float z);

		// rotate by radians
		void rotateParent(const glm::vec3 & change);
		void rotateParent(const float pitch, const float yaw, const float roll);
		// rotate by degrees
		void rotateDegParent(const glm::vec3 & change);
		void rotateDegParent(const float pitch, const float yaw, const float roll);

		void scaleParent(const float scalar);
		void scaleParent(const glm::vec3 & multiplier);
		void scaleParent(const float x, const float y, const float z);

		std::string name_;		// name_ need not be unique
		EntityID unique_ID_;	// unique_ID_ should be unique to a game engine execution

		std::string tag_;		// the lowest level tag applied to this entity (or "")
		std::string prefab_;	// the name of the prefab this entity inherits from (or "")

		// list of all sub/child entities
		std::vector<std::unique_ptr<Entity>> sub_entities_;

		// list of all components attached to this entity, components need not be active
		std::vector<std::unique_ptr<Component>> components_;

		Transform local_transform_;		// the transform of the entity relative to the parent
		Transform global_transform_;	// the transform of the entity relative to the world

		// isVisible, isEnabled, ...

		// utility method for deleting all components
		void deleteAllComponents() noexcept;
	};
}

