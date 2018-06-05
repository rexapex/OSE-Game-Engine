#pragma once
#include "Component.h"
#include "EntityList.h"
#include "OSE-Core/Game/IDManager.h"
#include "OSE-Core/Math/Transform.h"

namespace ose::entity
{
	using namespace math;
	using namespace game;

	class Entity
	{
	public:
		Entity(const std::string & name, const std::string & tag = "", const std::string & prefab = "");
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
		// provide const and non-const versions
		EntityList & sub_entities() { return sub_entities_; }
		const EntityList & sub_entities() const { return sub_entities_; }

		// get a list of all components
		const std::vector<std::unique_ptr<Component>> & get_components() const { return this->components_; }

		// add a component to the entity by component type
		// method constructs a new object of the given component type
		// template takes the type of component
		// method takes an array of contructor arguments
		template<class ComponentType, typename... Args>
		void addComponent(Args &&... params);

		// get the first component of specified type
		// returns raw pointer to component if one exists
		// returns nullptr if component of given type does not exist
		// entity class manages object, returned object should not be deleted (de-allocated)
		// IMPORTANT - template method so defined in header
		template<class ComponentType>
		ComponentType * getComponent() const
		{
			// check whether the type matches of each component
			for(auto && component : components_)
			{
				// if the type is correct, return a pointer to the component
				if(component->isClassType(ComponentType::Type)) {
					return static_cast<ComponentType*>(component.get());
				}
			}

			return nullptr;	// returns nullptr if no component of type given exists
		}

		// get a list of components of specified type
		// returns list of references
		// list will be empty if no component of given type exists
		// IMPORTANT - template method so defined in header
		template<class ComponentType>
		std::vector<ComponentType *> getComponents() const
		{
			std::vector<ComponentType *> matching_comps;

			for(auto && comp : components_)
			{
				// add every component which is/derives from the type given
				if(comp->isClassType(ComponentType::Type)) {
					matching_comps.emplace_back(static_cast<ComponentType*>(comp.get()));
				}
			}

			return matching_comps;
		}

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
		//std::vector<std::unique_ptr<Entity>> sub_entities_;
		EntityList sub_entities_;

		// list of all components attached to this entity, components need not be active
		std::vector<std::unique_ptr<Component>> components_;

		Transform local_transform_;		// the transform of the entity relative to the parent
		Transform global_transform_;	// the transform of the entity relative to the world

		// isVisible, isEnabled, ...

		// utility method for deleting all components
		void deleteAllComponents() noexcept;
	};
}

