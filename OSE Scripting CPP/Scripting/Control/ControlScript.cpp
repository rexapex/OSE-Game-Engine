#include "pch.h"
#include "ControlScript.h"

namespace ose::scripting
{
	ControlScript::ControlScript()
	{

	}

	ControlScript::~ControlScript()
	{

	}

	// If name and factory are given, links the control name to the control factory
	// Returns the map from control name to control factory
	std::unordered_map<std::string, ControlScriptFactory> const & ControlScript::GetSetControlScriptFactory(std::string const & name, ControlScriptFactory factory)
	{
		static std::unordered_map<std::string, ControlScriptFactory> ControlScriptFactories;
		if(factory != nullptr && name != "" && ControlScriptFactories.find(name) == ControlScriptFactories.end())
			ControlScriptFactories.emplace(name, factory);
		return ControlScriptFactories;
	}
}

