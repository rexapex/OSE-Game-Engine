#pragma once
#include "OSE-Core/Game/Game.h"

namespace ose
{
	using namespace game;

	// Represents the environment the game is being run in, all methods and fields are static
	class Env
	{
	private:
		// Uninstantiable class
		Env();
		~Env();

	public:
		// There is always one and only one game running per instance of OSE
		static const std::unique_ptr<Game> game;
	};
}
