#pragma once
#include "OSE-Core/Types.h"
#include "Component.h"
#include "EntityList.h"
#include "OSE-Core/Math/Transformable.h"

namespace ose::entity
{
	typedef uint32_t EntityID;	// NOTE - Might change this to uint64_t later

	class Entity : public EntityList, public math::Transformable<std::unique_ptr<Entity>>
	{
	public:
		Entity(const std::string & name, const std::string & tag = "", const std::string & prefab = "");
		virtual ~Entity() noexcept;
		Entity(const Entity & other) noexcept;
		Entity & operator=(const Entity & other) noexcept;
		Entity(Entity && other) noexcept = default;
		Entity & operator=(Entity && other) noexcept = default;

		const std::string & GetName() const { return this->name_; }
		const EntityID GetUniqueId() const { return this->unique_id_; }

		void SetName(const std::string & name) { this->name_ = name; }
		void SetTag(const std::string & tag) { this->tag_ = tag; }

		// get a list of all components
		const std::vector<std::unique_ptr<Component>> & GetComponents() const { return this->components_; }

		// add a component to the entity by component type
		// method constructs a new object of the given component type
		// template takes the type of component
		// method takes an array of contructor arguments
		template<class ComponentType, typename... Args>
		void AddComponent(Args &&... params)
		{
			components_.emplace_back( std::make_unique<ComponentType>(std::forward<Args>(params)...) );
		}

		// get the first component of specified type
		// returns raw pointer to component if one exists
		// returns nullptr if component of given type does not exist
		// entity class manages object, returned object should not be deleted (de-allocated)
		// IMPORTANT - template method so defined in header
		template<class ComponentType>
		ose::unowned_ptr<ComponentType> GetComponent() const
		{
			// check whether the type matches of each component
			for(auto && component : components_)
			{
				// if the type is correct, return a pointer to the component
				if(component->IsClassType(ComponentType::GetClassType())) {
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
		std::vector<ose::unowned_ptr<ComponentType>> GetComponents() const
		{
			std::vector<ComponentType *> matching_comps;

			for(auto && comp : components_)
			{
				// add every component which is/derives from the type given
				if(comp->IsClassType(ComponentType::GetClassType())) {
					matching_comps.emplace_back(static_cast<ComponentType*>(comp.get()));
				}
			}

			return matching_comps;
		}

		// remove the first component of specified type
		// returns true if component of given type is removed
		// returns false if no component of given type exists
		template<class ComponentType>
		bool RemoveComponent()
		{
			// no component can be removed if there are no components therefore return false
			if(components_.empty()) {
				return false;
			}

			// otherwise, find the first component of given type
			// search from beginning to end of list
			// return first component to return true from lambda
			auto & pos = std::find_if(components_.begin(), components_.end(), [type = ComponentType::GetClassType()] (auto & comp) {
				return comp->IsClassType(type);
			});

			// if a matching component is found, remove it then return true
			if(pos != components_.end()) {
				components_.erase(pos);
				return true;
			}

			// else, return false
			return false;
		}

		// remove all components which are of / are derived from given type
		// returns the number of removals
		template<class ComponentType>
		int32_t RemoveComponents()
		{
			// no component can be removed if there are no components therefore return 0
			if(components_.empty()) {
				return 0;
			}

			int32_t num_removals = 0;

			// use removeComponent method in a loop until no more components can be removed
			do {
				bool removed = this->RemoveComponent<ComponentType>();
				if(removed) num_removals ++;
			} while(removed);

			return num_removals;
		}

		// TODO - NEEDS SERIOUS TESTING, NO IDEA WHETHER THIS WORKS
		// remove the component passed from the entity
		// does NOT delete the component
		// returns true if the component is removed
		// returns false if the component does not belong to this entity
		bool RemoveComponent(const ose::unowned_ptr<Component> comp);

		// Get a list of transformable elements
		// Returns a list of child entities
		virtual const std::vector<std::unique_ptr<Entity>> & GetChildTransformables() override { return entities_; }

	private:

		std::string name_;		// name_ need not be unique
		EntityID unique_id_;	// unique_ID_ should be unique to a game engine execution

		std::string tag_;		// the lowest level tag applied to this entity (or "")
		std::string prefab_;	// the name of the prefab this entity inherits from (or "")

		// list of all components attached to this entity, components need not be active
		std::vector<std::unique_ptr<Component>> components_;

		math::Transform local_transform_;		// the transform of the entity relative to the parent
		math::Transform global_transform_;	// the transform of the entity relative to the world

		// isVisible, isEnabled, ...

		// utility method for deleting all components
		void DeleteAllComponents() noexcept;

		// Get the next available entity ID
		static EntityID NextEntityId()
		{
			static EntityID id { 0 };
			return id++;
		}
	};
}

