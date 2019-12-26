#include "pch.h"
#include "ScriptingFactoryCPP.h"
#include "ScriptingEngineCPP.h"

namespace ose::scripting
{
	std::unique_ptr<ScriptingEngine> ScriptingFactoryCPP::NewScriptingEngine()
	{
		return std::make_unique<ScriptingEngineCPP>();
	}
}
