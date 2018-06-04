#include "stdafx.h"
#include "Env.h"

namespace ose
{
	Env::Env()
	{

	}

	Env::~Env()
	{

	}

	// construct the game object, projects & scenes will be loaded later
	const std::unique_ptr<Game> Env::game = std::make_unique<Game>();
}
