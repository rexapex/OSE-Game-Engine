#include "stdafx.h"

#include "../OSE Scripting CPP/Scripting/EntityScript.h"
#include "../OSE V2/OSE-Core/Entity/Entity.h"

using namespace ose::scripting;
using namespace ose::entity;

// Set the name of the custom component
#define NAME PlayerMovement

// Define the data stored for each custom component object
DATA
(
	Entity & entity;
)

// Define the engine used to process the pool of custom component objects
ENGINE
(
	void Init()
	{
		std::cout << "Initialising custom engine\n";
	}

	void Update()
	{
		for(auto & data : data_array_)
		{
			data.entity.Translate2d(0.1f, 0);
		//	std::cout << "Entity Translation: " << data.entity.GetLocalTransform().GetPosition().x << "\n";
		}
	}

	void AddCustomComponent(Entity & entity, CustomComponent & comp)
	{
		std::cout << "Adding custom component\n";
		data_array_.emplace_back(PlayerMovementData{entity});
	}
)
