#include "pch.h"
#include "ScriptPoolCPP.h"
#include "OSE-Core/Entity/Component/CustomComponent.h"
#include "OSE-Core/Scripting/ControlSettings.h"
#include "OSE-Core/Scripting/CustomControl.h"

namespace ose::scripting
{
	ScriptPoolCPP::ScriptPoolCPP()
	{

	}

	ScriptPoolCPP::~ScriptPoolCPP()
	{

	}

	// Initialise the script pool
	void ScriptPoolCPP::Init()
	{
		// Create custom engines from the custom engine factories map
		for(auto const & factory : CustomEngine::GetSetCustomEngineFactory())
		{
			auto engine { factory.second() };
			if(engine != nullptr)
				custom_engines_.emplace_back(std::move(engine));
		}
	}

	// Add a custom engine component to the script pool
	void ScriptPoolCPP::AddCustomComponent(Entity & entity, unowned_ptr<CustomComponent> comp)
	{
		// Attempt to find the custom engine for the component
		std::string const & type_name = comp->GetComponentTypeName();
		auto iter = std::find_if(custom_engines_.begin(), custom_engines_.end(), [type_name](auto & engine) {
			return engine->GetComponentTypeName() == type_name;
		});
		// Add the component to the engine if it exists
		if(iter != custom_engines_.end())
		{
			(*iter)->AddCustomComponent(entity, *comp);
		}
		else
		{
			ERROR_LOG("Error: Failed to find custom engine for custom component: " << type_name);
		}
	}

	// Apply a control settings object to initialise an array of controls
	void ScriptPoolCPP::ApplyControlSettings(ControlSettings const & settings, bool persistent)
	{
		auto factories = ControlScript::GetSetControlScriptFactory();

		auto add_control = [this, &factories](CustomControl const & control, std::vector<std::unique_ptr<ControlScript>> & array) {
			// Attempt to find the control script
			std::string const & type_name = control.GetControlTypeName();
			auto iter = std::find_if(controls_.begin(), controls_.end(), [type_name](auto & control) {
				return control->GetControlTypeName() == type_name;
			});
			auto iter2 = std::find_if(deferred_controls_.begin(), deferred_controls_.end(), [type_name](auto & control) {
				return control->GetControlTypeName() == type_name;
			});
			auto iter3 = std::find_if(persistent_controls_.begin(), persistent_controls_.end(), [type_name](auto & control) {
				return control->GetControlTypeName() == type_name;
			});
			auto iter4 = std::find_if(deferred_persistent_controls_.begin(), deferred_persistent_controls_.end(), [type_name](auto & control) {
				return control->GetControlTypeName() == type_name;
			});
			// Add the control if it doesn't already exist
			if(iter == controls_.end() && iter2 == deferred_controls_.end()
				&& iter3 == persistent_controls_.end() && iter4 == deferred_persistent_controls_.end())
			{
				auto factory_iter = factories.find("New"+control.GetControlTypeName()+"Control");
				if(factory_iter != factories.end())
					array.emplace_back(factory_iter->second());
				else
					ERROR_LOG("Error: Failed to find custom control: " << control.GetControlTypeName());
			}
			else
			{
				ERROR_LOG("Error: Attempting to create a control which already exists");
			}
		};

		if(persistent)
		{
			// If the persistent controls are changing it means a new project has been loaded, therefore, reset all the controls
			controls_.clear();
			deferred_controls_.clear();
			persistent_controls_.clear();
			deferred_persistent_controls_.clear();
			// Apply the settings object
			for(auto const & control : settings.controls_)
			{
				add_control(control, persistent_controls_);
			}
			for(auto const & control : settings.deferred_controls_)
			{
				add_control(control, deferred_persistent_controls_);
			}
		}
		else
		{
			// If the non-persistent controls are changing it means a new scene has been activated, therefore, reset all non-persistent controls
			controls_.clear();
			deferred_controls_.clear();
			// Apply the settings object
			for(auto const & control : settings.controls_)
			{
				add_control(control, controls_);
			}
			for(auto const & control : settings.deferred_controls_)
			{
				add_control(control, deferred_controls_);
			}
		}
	}

	// Remove a custom engine component from the script pool
	void ScriptPoolCPP::RemoveCustomComponent(unowned_ptr<CustomComponent> comp)
	{
		// Attempt to find the custom engine for the component
		std::string const & type_name = comp->GetComponentTypeName();
		auto iter = std::find_if(custom_engines_.begin(), custom_engines_.end(), [type_name](auto & engine) {
			return engine->GetComponentTypeName() == type_name;
		});
		// Remove the component to the engine if it exists
		if(iter != custom_engines_.end())
		{
			(*iter)->RemoveCustomComponent(*comp);
		}
	}
}
