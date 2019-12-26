#pragma once
#include "ScriptPool.h"

namespace ose::scripting
{
	class ScriptingEngine
	{
	public:
		ScriptingEngine();
		virtual ~ScriptingEngine();

		// Initialise all of the custom engines and controls in the script pool
		virtual void Init() = 0;

		// Update all of the custom engines and controls in the script pool
		virtual void Update() = 0;

		// Get a reference to the script pool
		// TODO - Refactor out returns by non-const reference
		virtual ScriptPool & GetScriptPool() = 0;
	};
}
