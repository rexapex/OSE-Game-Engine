#pragma once

namespace ose
{
	namespace entity
	{
		class Entity;
	}
}

namespace ose::scripting
{
	class ScriptPool
	{
		ScriptPool() {}
		virtual ~ScriptPool() {}

		// Add a custom engine component to the script pool
		virtual void AddCustomEngine(ose::entity::Entity & entity, ose::unowned_ptr<) = 0;
	};
}
