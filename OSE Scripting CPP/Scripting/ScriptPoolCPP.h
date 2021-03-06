#pragma once
#include "OSE-Core/Scripting/ScriptPool.h"
#include "Custom Engine/CustomEngine.h"
#include "Control/ControlScript.h"

namespace ose
{
	class CustomComponent;
	struct ControlSettings;
}

namespace ose::scripting
{
	class ScriptPoolCPP final : public ScriptPool
	{
	public:
		ScriptPoolCPP();
		~ScriptPoolCPP();

		// Initialise the script pool
		void Init();

		// Add a custom engine component to the script pool
		void AddCustomComponent(Entity * entity, CustomComponent * comp) override;

		// Remove a custom engine component from the script pool
		void RemoveCustomComponent(CustomComponent * comp) override;

		// Apply a control settings object to initialise an array of controls
		void ApplyControlSettings(ControlSettings const & settings, bool persistent=false) override;

		// Get the list of custom engines
		std::vector<uptr<CustomEngine>> const & GetCustomEngines() const { return custom_engines_; }

		// Get the lists of controls
		std::vector<uptr<ControlScript>> const & GetControls() const { return controls_; }
		std::vector<uptr<ControlScript>> const & GetDeferredControls() const { return deferred_controls_; }
		std::vector<uptr<ControlScript>> const & GetPersistentControls() const { return persistent_controls_; }
		std::vector<uptr<ControlScript>> const & GetDeferredPersistentControls() const { return deferred_persistent_controls_; }

	private:
		std::vector<uptr<CustomEngine>> custom_engines_;
		std::vector<uptr<ControlScript>> controls_;
		std::vector<uptr<ControlScript>> deferred_controls_;
		std::vector<uptr<ControlScript>> persistent_controls_;
		std::vector<uptr<ControlScript>> deferred_persistent_controls_;
	};
}
