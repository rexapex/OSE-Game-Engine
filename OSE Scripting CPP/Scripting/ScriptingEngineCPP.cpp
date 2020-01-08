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
		// TODO - Ensure new scripts are actually initialised (this method currently only called once)
		for(auto & control : script_pool_.GetPersistentControls())
			control->Init(game);
		for(auto & control : script_pool_.GetControls())
			control->Init(game);
		for(auto & engine : script_pool_.GetCustomEngines())
			engine->Init(game);
		for(auto & control : script_pool_.GetDeferredControls())
			control->Init(game);
		for(auto & control : script_pool_.GetDeferredPersistentControls())
			control->Init(game);
	}

	// Update all of the custom engines and controls in the script pool
	void ScriptingEngineCPP::Update()
	{
		for(auto & control : script_pool_.GetPersistentControls())
			control->Update();
		for(auto & control : script_pool_.GetControls())
			control->Update();
		for(auto & engine : script_pool_.GetCustomEngines())
			engine->Update();
		for(auto & control : script_pool_.GetDeferredControls())
			control->Update();
		for(auto & control : script_pool_.GetDeferredPersistentControls())
			control->Update();
	}
}
