#pragma once

#include "OSE-Core/Game/GameObject.h"
#include "OSE-Core/Scripting/ControlSettings.h"

namespace ose
{
	class Scene : public GameObject
	{
	public:
		Scene(const std::string & name, const ControlSettings & control_settings);
		virtual ~Scene() noexcept;
		Scene(const Scene & other) noexcept;
		Scene(Scene &&) noexcept = default;
		Scene & operator=(Scene &) = delete;
		Scene & operator=(Scene &&) = delete;
		
		// Get the name of the scene
		const std::string & GetName() const { return this->name_; }

		// Get the control script settings of the scene
		const ControlSettings & GetControlSettings() const { return control_settings_; }

		// DEBUG METHODS
		// TODO - REMOVE WHEN READY
		void Print();
		void PrintEntity(const Entity & e, int32_t level);

	private:
		// Name of the scene
		// Must be unique among scenes
		std::string name_;

		// Array of control scripts required by the scene in the order they should be run
		ControlSettings control_settings_;
	};
}

