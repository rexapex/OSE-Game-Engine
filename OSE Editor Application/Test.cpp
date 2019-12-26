#include "stdafx.h"

#include "../OSE Scripting CPP/Scripting/EntityScript.h"

using namespace ose::scripting;
using namespace ose::entity;

// Set the name of the custom component
#define NAME PlayerMovement

// Define the data stored for each custom component object
DATA
(
	int x;
	int y;
)

// Define the engine used to process the pool of custom component objects
ENGINE
(
	void AddCustomComponent(Entity & entity, CustomComponent & comp)
	{
		
	}

	void Init()
	{
		
	}

	void Update()
	{
		std::cout << "Updating script\n";
	}
)
