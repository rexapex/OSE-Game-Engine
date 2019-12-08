#pragma once

namespace ose
{
	namespace game
	{
		class Game;
	}
}

namespace ose
{
	// Represents the environment the game is being run in, all methods and fields are static
	class Env
	{
	public:
		Env(Env const &) = delete;
		Env & operator=(Env const &) = delete;

		static ose::game::Game & GetGame()
		{
			return *GetInstance().game_;
		}

	private:
		// Uninstantiable class
		Env();
		~Env();

		static Env & GetInstance()
		{
			static Env env;
			return env;
		}

		std::unique_ptr<ose::game::Game> game_;
	};
}
