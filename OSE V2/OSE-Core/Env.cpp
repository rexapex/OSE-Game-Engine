#include "stdafx.h"
#include "Env.h"
#include "EngineReferences.h"
#include "OSE-Core/Game/Game.h"

namespace ose
{
	Env::Env()
	{
		// Construct the game object, projects & scenes will be loaded later
		game_ = std::make_unique<Game>();
	}

	Env::~Env()
	{

	}
}
