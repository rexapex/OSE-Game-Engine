#pragma once
#include "OSE-Core/Game/Game.h"

namespace ose
{
	using namespace game;

	// represents the environment the game is being run in, all methods and fields are static
	class Env
	{
	private:
		// uninstantiable class
		Env();
		~Env();

	public:
		// there is always one and only one game running per instance of OSE
		static const std::unique_ptr<Game> game;
	};
}
