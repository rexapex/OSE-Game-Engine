#pragma once
#include "Component.h"
#include "OSE-Core/Game/IDManager.h"
#include "OSE-Core/Math/Transform.h"

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

		const std::string & get_name() const {return this->name_;}
		const uint32_t get_unique_ID() const {return this->unique_ID_;}

		void set_name(const std::string & name) {this->name_ = name;}	//should only be called once named is confirmed as "unique" (see name_ definition)
		void set_tag(const std::string & tag) { this->tag_ = tag; }

		//non-const references allow for quicker loading by emplacing items directly into vectors
		const std::vector<std::unique_ptr<Entity>> & get_sub_entities() const { return this->sub_entities_; }

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

		// get a list of all components
		const std::vector<std::unique_ptr<Component>> & get_components() const { return this->components_; }

		// add a component to the entity by component type
		// method constructs a new object of the given component type
		// template takes the type of component
		// method takes an array of contructor arguments
		template<class ComponentType, typename... Args>
		void addComponent(Args &&... params);

		// get the first component of specified type
		// returns pointer to component if one exists
		// entity class manages pointer, returned pointer should not be deleted (de-allocated)
		// returns nullptr if no component of type given exists
		template<class ComponentType>
		ComponentType * getComponent() const;

		//read-only transform relative the parent entity/world if no parent exists
		const Transform & get_local_transform() const { return this->local_transform_; }

		//read-only transform relative to the world
		const Transform & get_global_transform() const { return this->global_transform_; }

		//modify the local and global transform of the entity
		void translate(const glm::vec3 & translation);
		void translate(const float x, const float y, const float z);

		//rotate by radians
		void rotate(const glm::vec3 & change);
		void rotate(const float pitch, const float yaw, const float roll);
		//rotate by degrees
		void rotateDeg(const glm::vec3 & change);
		void rotateDeg(const float pitch, const float yaw, const float roll);

		void scale(const float scalar);
		void scale(const glm::vec3 & multiplier);
		void scale(const float x, const float y, const float z);

		//modify just the global transform of the entity
		void translateParent(const glm::vec3 & translation);
		void translateParent(const float x, const float y, const float z);

		//rotate by radians
		void rotateParent(const glm::vec3 & change);
		void rotateParent(const float pitch, const float yaw, const float roll);
		//rotate by degrees
		void rotateDegParent(const glm::vec3 & change);
		void rotateDegParent(const float pitch, const float yaw, const float roll);

		void scaleParent(const float scalar);
		void scaleParent(const glm::vec3 & multiplier);
		void scaleParent(const float x, const float y, const float z);

	private:
		std::string name_;		//name_ should be unique to a scene file
		uint32_t unique_ID_;	//unique_ID_ should be unique to a game engine execution

		std::string tag_;		//the lowest level tag applied to this entity (or "")
		std::string prefab_;	//the name of the prefab this entity inherits from (or "")

		// list of all sub/child entities
		std::vector<std::unique_ptr<Entity>> sub_entities_;

		// list of all components attached to this entity, components need not be active
		std::vector<std::unique_ptr<Component>> components_;

		Transform local_transform_;		//the transform of the entity relative to the parent
		Transform global_transform_;	//the transform of the entity relative to the world

		//isVisible, isEnabled, ...

		// utility method for deleting all components
		void deleteAllComponents() noexcept;
	};
}

