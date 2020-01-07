#pragma once

namespace ose
{
	class Entity;
	class CustomComponent;

	class ScriptPool
	{
	public:
		ScriptPool() {}
		virtual ~ScriptPool() {}

		// Add a custom engine component to the script pool
		virtual void AddCustomComponent(Entity & entity, unowned_ptr<CustomComponent> comp) = 0;

		// Remove a custom engine component from the script pool
		virtual void RemoveCustomComponent(unowned_ptr<CustomComponent> comp) = 0;
	};
}
