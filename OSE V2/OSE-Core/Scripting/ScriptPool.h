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
	};
}
