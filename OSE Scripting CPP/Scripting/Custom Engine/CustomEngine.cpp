#include "pch.h"
#include "CustomEngine.h"

namespace ose::scripting
{
	CustomEngine::CustomEngine()
	{
	
	}

	CustomEngine::~CustomEngine()
	{
		
	}

	// If name and factory are given, links the engine name to the engine factory
	// Returns the map from engine name to engine factory
	std::unordered_map<std::string, CustomEngineFactory> const & CustomEngine::GetSetCustomEngineFactory(std::string const & name, CustomEngineFactory factory)
	{
		static std::unordered_map<std::string, CustomEngineFactory> CustomEngineFactories;
		if(factory != nullptr && name != "" && CustomEngineFactories.find(name) == CustomEngineFactories.end())
			CustomEngineFactories.emplace(name, factory);
		return CustomEngineFactories;
	}
}
