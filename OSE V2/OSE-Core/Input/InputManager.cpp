#include "stdafx.h"
#include "InputManager.h"

namespace ose
{
	InputManager::InputManager()
	{
	
	}

	InputManager::~InputManager()
	{
	
	}

	// Add a new boolean input to the input manager
	// Returns true iff the input is added successfully
	bool InputManager::AddBooleanInput(std::string const & name, EInputType primary, EInputType secondary)
	{
		auto iter = boolean_inputs_.find(name);
		if(iter != boolean_inputs_.end())
			return false;
		boolean_inputs_.emplace(name, BooleanInput{ primary, secondary, false, false });
		return true;
	}

	// Add a new axis input to the input manager
	// Returns true iff the input is added successfully
	bool InputManager::AddAxisInput(std::string const & name, EInputType pos_primary, EInputType neg_primary, EInputType pos_secondary, EInputType neg_secondary)
	{
		auto iter = axis_inputs_.find(name);
		if(iter != axis_inputs_.end())
			return false;
		axis_inputs_.emplace(name, AxisInput{ pos_primary, pos_secondary, neg_primary, neg_secondary, 0.0 });
		return true;
	}
}
