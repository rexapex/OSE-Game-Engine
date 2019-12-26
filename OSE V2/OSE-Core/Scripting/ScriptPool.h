#pragma once

namespace ose
{
	namespace entity
	{
		class Entity;
		class CustomComponent;
	}
}

namespace ose::scripting
{
	class ScriptPool
	{
		ScriptPool() {}
		virtual ~ScriptPool() {}

		// Add a custom engine component to the script pool
		virtual void AddCustomEngine(ose::entity::Entity & entity, ose::unowned_ptr<ose::entity::CustomComponent> comp) = 0;
	};
}
