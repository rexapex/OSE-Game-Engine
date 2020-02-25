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

	// Initialise all of the custom engines  in the script pool
	void ScriptingEngineCPP::InitCustomEngines(unowned_ptr<Game> game)
	{
		for(auto & engine : script_pool_.GetCustomEngines())
			engine->Init(game);
	}

	// Initialise all of the persistent controls in the script pool
	void ScriptingEngineCPP::InitPersistentControls(unowned_ptr<Game> game)
	{
		for(auto & control : script_pool_.GetPersistentControls())
			control->Init(game);
		for(auto & control : script_pool_.GetDeferredPersistentControls())
			control->Init(game);
	}

	// Initialise all of the non-persistent (scene) controls in the script pool
	void ScriptingEngineCPP::InitSceneControls(unowned_ptr<Game> game)
	{
		for(auto & control : script_pool_.GetControls())
			control->Init(game);
		for(auto & control : script_pool_.GetDeferredControls())
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
