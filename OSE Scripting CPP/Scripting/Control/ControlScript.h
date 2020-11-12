#pragma once

namespace ose
{
	class Game;
}

namespace ose::scripting
{
	class ControlScript;
	using ControlScriptFactory = uptr<ControlScript>(*)();

	class ControlScript
	{
	protected:
		ControlScript();

	public:
		virtual ~ControlScript();

		virtual std::string GetControlTypeName() const = 0;
		
		virtual void Init(Game * game) {}
		virtual void Update() {}

		// If name and factory are given, links the control name to the control factory
		// Returns the map from control name to control factory
		static std::unordered_map<std::string, ControlScriptFactory> const & GetSetControlScriptFactory(std::string const & name = "", ControlScriptFactory factory = nullptr);
	};
}
