#include "stdafx.h"
#include "ComponentList.h"
#include "Component.h"

namespace ose
{
	ComponentList::ComponentList() {}
	ComponentList::~ComponentList() {}

	ComponentList::ComponentList(ComponentList const & other) noexcept
	{
		// TODO - remove any existing components
		this->DeleteAllComponents();		// NOTE - before this can be done, the components must be removed from engines
											// copy each component from other
		for(auto const & comp : other.components_)
		{
			// Component base class won't compile if abstract so check for it here instead (and elsewhere)
			if(comp->IsClassType(Component::GetClassType())) {
				// using a clone method prevents slicing
				this->components_.emplace_back(comp->Clone());
			}
		}
	}

	// utility method for deleting all components
	void ComponentList::DeleteAllComponents() noexcept
	{
		// TODO - delete components from their respective engines
		this->components_.clear();
	}

	// remove the component passed from the entity
	// does NOT delete the component
	// returns true if the component is removed
	// returns false if the component does not belong to this entity
	bool ComponentList::RemoveComponent(Component const * comp)
	{
		// No component can be removed if there are no components therefore return false
		// Nullptr cannot be in list of components_ therefore exit if comp == nullptr
		if(components_.empty() || comp == nullptr) {
			return false;
		}

		// TODO - test whether this actually works
		// NOTE - remove moves removed elements to end and returns the new end as an iterator
		// NOTE - erase then deletes element between first arg and last arg from the vector
		size_t size_before { components_.size() };
		components_.erase(std::remove_if(components_.begin(), components_.end(), [this, comp] (auto & component) {
			return component.get() == comp;
		}), components_.end());
		return (size_before != components_.size());
	}
}
