#include "pch.h"
#include "ScriptingEngineCPP.h"

namespace ose::scripting
{
	ScriptingEngineCPP::ScriptingEngineCPP() : ScriptingEngine()
	{
		script_pool_.Init();
	}
	
	ScriptingEngineCPP::~ScriptingEngineCPP()
	{
	
	}

	// Initialise all of the custom engines and controls in the script pool
	void ScriptingEngineCPP::Init(unowned_ptr<Game> game)
	{
		for(auto & engine : script_pool_.GetCustomEngines())
		{
			engine->Init(game);
		}
	}

	// Update all of the custom engines and controls in the script pool
	void ScriptingEngineCPP::Update()
	{
		for(auto & engine : script_pool_.GetCustomEngines())
		{
			engine->Update();
		}
	}
}
