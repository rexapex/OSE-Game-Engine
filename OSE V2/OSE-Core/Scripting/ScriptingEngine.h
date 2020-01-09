#pragma once
#include "ScriptPool.h"

namespace ose
{
	class Game;

	class ScriptingEngine
	{
	public:
		ScriptingEngine();
		virtual ~ScriptingEngine();

		// Initialise all of the custom engines in the script pool
		virtual void InitCustomEngines(unowned_ptr<Game> game) = 0;

		// Initialise all of the persistent controls in the script pool
		virtual void InitPersistentControls(unowned_ptr<Game> game) = 0;

		// Initialise all of the non-persistent (scene) controls in the script pool
		virtual void InitSceneControls(unowned_ptr<Game> game) = 0;

		// Update all of the custom engines and controls in the script pool
		virtual void Update() = 0;

		// Get a reference to the script pool
		// TODO - Refactor out returns by non-const reference
		virtual ScriptPool & GetScriptPool() = 0;
	};
}
