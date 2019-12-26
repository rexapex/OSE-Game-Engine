#include "pch.h"
#include "ScriptingEngineCPP.h"

namespace ose::scripting
{
	ScriptingEngineCPP::ScriptingEngineCPP() : ScriptingEngine()
	{
	
	}
	
	ScriptingEngineCPP::~ScriptingEngineCPP()
	{
	
	}

	// Initialise all of the custom engines and controls in the script pool
	void ScriptingEngineCPP::Init()
	{
		script_pool_.Init();

		for(auto & engine : script_pool_.GetCustomEngines())
		{
			engine->Init();
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
