#include "stdafx.h"
#include "InputManager.h"
#include "InputSettings.h"

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

	// Returns true iff boolean input is triggered
	// Returns false if boolean input doesn't exist
	bool InputManager::IsBooleanInputTriggered(std::string const & name) const
	{
		auto iter = boolean_inputs_.find(name);
		if(iter != boolean_inputs_.end())
			return iter->second.triggered_;
		return false;
	}

	// Returns true iff boolean input is changing from an un-triggered state to a triggered state
	// Returns false if boolean input doesn't exist
	bool InputManager::IsBooleanInputTriggering(std::string const & name) const
	{
		auto iter = boolean_inputs_.find(name);
		if(iter != boolean_inputs_.end())
			return iter->second.triggered_ && !iter->second.triggered_last_update_;
		return false;
	}

	// Returns true iff boolean input is changing from a triggered state to an un-triggered state
	// Returns false if boolean input doesn't exist
	bool InputManager::IsBooleanInputUntriggering(std::string const & name) const
	{
		auto iter = boolean_inputs_.find(name);
		if(iter != boolean_inputs_.end())
			return !iter->second.triggered_ && iter->second.triggered_last_update_;
		return false;
	}

	// Returns value of axis input in range [-1, 1]
	// Returns 0 if axis input doesn't exist
	double InputManager::GetAxisValue(std::string const & name) const
	{
		auto iter = axis_inputs_.find(name);
		if(iter != axis_inputs_.end())
			return iter->second.value_;
		return 0;
	}
	
	// Set input type to triggered or un-triggered
	void InputManager::SetInputType(EInputType type, bool triggered)
	{
		// TODO - Consider also maintaining map from type to boolean/axis name to allow faster triggering of inputs
		for(auto & pair : boolean_inputs_)
		{
			if(pair.second.primary_ == type || pair.second.secondary_ == type)
			{
				pair.second.triggered_last_update_ = pair.second.triggered_;
				pair.second.triggered_ = triggered;
			}
		}

		for(auto & pair : axis_inputs_)
		{
			if(pair.second.pos_primary_ == type || pair.second.pos_secondary_ == type)
			{
				pair.second.pos_value_ = triggered ? 1.0 : 0.0;
				pair.second.value_ = pair.second.pos_value_ - pair.second.neg_value_;
			}
			else if(pair.second.neg_primary_ == type || pair.second.neg_secondary_ == type)
			{
				pair.second.neg_value_ = triggered ? 1.0 : 0.0;
				pair.second.value_ = pair.second.pos_value_ - pair.second.neg_value_;
			}
		}
	}

	// Apply an InputSettings object to the input manager
	// New inputs will be added and existing inputs will be overriden
	void InputManager::ApplyInputSettings(InputSettings const & settings)
	{
		for(auto pair : settings.boolean_inputs_)
		{
			boolean_inputs_[pair.first] = pair.second;
		}
		for(auto pair : settings.axis_inputs_)
		{
			axis_inputs_[pair.first] = pair.second;
		}
	}

	// Clear all boolean and axis inputs
	void InputManager::ClearInputs()
	{
		boolean_inputs_.clear();
		axis_inputs_.clear();
	}
}
