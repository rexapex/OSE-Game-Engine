#pragma once
#include "OSE-Core/Scripting/ScriptingFactory.h"

namespace ose
{
	class ScriptingEngine;
}

namespace ose::scripting
{
	class ScriptingFactoryCPP : public ScriptingFactory
	{
	public:
		constexpr ScriptingFactoryCPP() : ScriptingFactory() {}
		virtual ~ScriptingFactoryCPP() {}
		ScriptingFactoryCPP(ScriptingFactoryCPP &) = delete;
		ScriptingFactoryCPP & operator=(ScriptingFactoryCPP &) = delete;
		ScriptingFactoryCPP(ScriptingFactoryCPP &&) = default;
		ScriptingFactoryCPP & operator=(ScriptingFactoryCPP &&) = default;

		std::unique_ptr<ScriptingEngine> NewScriptingEngine() override;
	};
}
