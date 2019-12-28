#pragma once
#include "AxisInput.h"
#include "BooleanInput.h"
#include "EInputType.h"

namespace ose
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

	private:
		std::unordered_map<std::string, BooleanInput> boolean_inputs_;
		std::unordered_map<std::string, AxisInput> axis_inputs_;
	};
}
