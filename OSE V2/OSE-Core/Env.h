#pragma once

namespace ose
{
	class Game;

	// Represents the environment the game is being run in, all methods and fields are static
	class Env
	{
	public:
		Env(Env const &) = delete;
		Env & operator=(Env const &) = delete;

		static Game & GetGame()
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

		std::unique_ptr<Game> game_;
	};
}
