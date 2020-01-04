#pragma once
#include "OSE-Core/Scripting/ScriptingEngine.h"
#include "ScriptPoolCPP.h"

namespace ose::scripting
{
	class ScriptingEngineCPP final : public ScriptingEngine
	{
	public:
		ScriptingEngineCPP();
		~ScriptingEngineCPP();

		// Initialise all of the custom engines and controls in the script pool
		void Init(unowned_ptr<Game> game) override;

		// Update all of the custom engines and controls in the script pool
		void Update() override;

		// Get a reference to the script pool
		// TODO - Refactor out returns by non-const reference
		ScriptPool & GetScriptPool() override { return script_pool_; }

	private:
		ScriptPoolCPP script_pool_;
	};
}
