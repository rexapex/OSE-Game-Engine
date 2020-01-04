#pragma once

namespace ose
{
	class ScriptingEngine;

	class ScriptingFactory
	{
	public:
		constexpr ScriptingFactory() {}
		virtual ~ScriptingFactory() {}
		ScriptingFactory(ScriptingFactory &) = delete;
		ScriptingFactory & operator=(ScriptingFactory &) = delete;
		ScriptingFactory(ScriptingFactory &&) = default;
		ScriptingFactory & operator=(ScriptingFactory &&) = default;

		virtual std::unique_ptr<ScriptingEngine> NewScriptingEngine() = 0;
	};
}
