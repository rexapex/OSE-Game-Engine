#pragma once
#include "AxisInput.h"
#include "BooleanInput.h"
#include "EInputType.h"

namespace ose::input
{
	class InputManager final
	{
	public:
		InputManager();
		~InputManager();
		InputManager(InputManager &) = delete;
		InputManager & operator=(InputManager &) = delete;
		InputManager(InputManager &&) = default;
		InputManager & operator=(InputManager &&) = default;

		// Add a new boolean input to the input manager
		// Name must be unique among all boolean inputs
		// Returns true iff the input is added successfully
		bool AddBooleanInput(std::string const & name, EInputType primary, EInputType secondary = EInputType::NONE);

		// Add a new axis input to the input manager
		// Name must be unique among all axis inputs
		// Returns true iff the input is added successfully
		bool AddAxisInput(std::string const & name, EInputType pos_primary, EInputType neg_primary, EInputType pos_secondary = EInputType::NONE, EInputType neg_secondary = EInputType::NONE);

		// Returns true iff boolean input is triggered
		// Returns false if boolean input doesn't exist
		bool IsBooleanInputTriggered(std::string const & name);

		// Returns true iff boolean input is changing from an un-triggered state to a triggered state
		// Returns false if boolean input doesn't exist
		bool IsBooleanInputTriggering(std::string const & name);

		// Returns true iff boolean input is changing from a triggered state to an un-triggered state
		// Returns false if boolean input doesn't exist
		bool IsBooleanInputUntriggering(std::string const & name);

		// Returns value of axis input in range [-1, 1]
		// Returns 0 if axis input doesn't exist
		double GetAxisValue(std::string const & name);

		// Set input type to triggered or un-triggered
		void SetInputType(EInputType type, bool triggered);

	private:
		std::unordered_map<std::string, BooleanInput> boolean_inputs_;
		std::unordered_map<std::string, AxisInput> axis_inputs_;
	};
}
