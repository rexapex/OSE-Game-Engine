#pragma once

namespace ose
{
	class Entity;
	class CustomComponent;
	class ControlSettings;

	class ScriptPool
	{
	public:
		ScriptPool() {}
		virtual ~ScriptPool() {}

		// Add a custom engine component to the script pool
		virtual void AddCustomComponent(Entity & entity, unowned_ptr<CustomComponent> comp) = 0;

		// Apply a control settings object to initialise an array of controls
		virtual void ApplyControlSettings(ControlSettings const & settings, bool persistent=false) = 0;
	};
}
