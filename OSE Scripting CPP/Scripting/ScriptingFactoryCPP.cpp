#include "pch.h"
#include "ScriptingFactoryCPP.h"
#include "ScriptingEngineCPP.h"

namespace ose::scripting
{
	uptr<ScriptingEngine> ScriptingFactoryCPP::NewScriptingEngine()
	{
		return ose::make_unique<ScriptingEngineCPP>();
	}
}
