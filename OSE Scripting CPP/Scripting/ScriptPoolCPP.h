#pragma once
#include "OSE-Core/Scripting/ScriptPool.h"
#include "Custom Engine/CustomEngine.h"

namespace ose::scripting
{
	class CustomComponent;

	class ScriptPoolCPP final : public ScriptPool
	{
	public:
		ScriptPoolCPP();
		~ScriptPoolCPP();

		// Initialise the script pool
		void Init();

		// Add a custom engine component to the script pool
		void AddCustomComponent(ose::entity::Entity & entity, ose::unowned_ptr<ose::entity::CustomComponent> comp) override;

		// Get the list of custom engines
		std::vector<std::unique_ptr<CustomEngine>> const & GetCustomEngines() const { return custom_engines_; }

	private:
		std::vector<std::unique_ptr<CustomEngine>> custom_engines_;
	};
}
