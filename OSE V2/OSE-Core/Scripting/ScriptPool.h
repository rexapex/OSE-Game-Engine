#pragma once

namespace ose
{
	class Entity;
	class CustomComponent;
	class CustomControl;

	class ScriptPool
	{
	public:
		ScriptPool() {}
		virtual ~ScriptPool() {}

		// Add a custom engine component to the script pool
		virtual void AddCustomComponent(Entity & entity, unowned_ptr<CustomComponent> comp) = 0;

		// Add a custom control to the script pool
		virtual void AddCustomControl(unowned_ptr<CustomControl> control) = 0;
	};
}
