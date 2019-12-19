#pragma once

namespace ose::scripting
{
	class ScriptPool
	{
		ScriptPool() {}
		virtual ~ScriptPool() {}

		// Add a custom engine component to the script pool
		virtual void AddCustomEngine() = 0;
	};
}
