#include "pch.h"
#include "ScriptPoolCPP.h"
#include "OSE-Core/Entity/Component/CustomComponent.h"

namespace ose::scripting
{
	ScriptPoolCPP::ScriptPoolCPP()
	{

	}

	ScriptPoolCPP::~ScriptPoolCPP()
	{

	}

	// Initialise the script pool
	void ScriptPoolCPP::Init()
	{
		// Create custom engines from the custom engine factories map
		for(auto const & factory : CustomEngine::GetSetCustomEngineFactory())
		{
			auto engine { factory.second() };
			if(engine != nullptr)
				custom_engines_.emplace_back(std::move(engine));
		}
	}

	// Add a custom engine component to the script pool
	void ScriptPoolCPP::AddCustomComponent(ose::entity::Entity & entity, ose::unowned_ptr<ose::entity::CustomComponent> comp)
	{
		// Attempt to find the custom engine for the component
		std::string const & type_name = comp->GetComponentTypeName();
		auto iter = std::find_if(custom_engines_.begin(), custom_engines_.end(), [type_name](auto & engine) {
			return engine->GetComponentTypeName() == type_name;
		});
		// Add the component to the engine if it exists
		if(iter != custom_engines_.end())
		{
	//		(*iter)->
		}
		else
		{
			ERROR_LOG("Error: Failed to find custom engine for custom component: " << type_name);
		}
	}
}
