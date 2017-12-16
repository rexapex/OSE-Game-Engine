#pragma once

#include <thread>
#include "stdafx.h"
#include "GameThread.h"

namespace origami_sheep_engine
{
	class GameThreadSTD : public GameThread
	{
	public:
		GameThreadSTD(const std::string & name, std::function<void(std::string &)> get_new_task_, std::mutex & mu, std::condition_variable & work_to_do);
		~GameThreadSTD() noexcept;

		//copy constructors
		GameThreadSTD(const GameThreadSTD &) = delete;
		GameThreadSTD & operator=(const GameThreadSTD &) = delete;

		//move constructors
		GameThreadSTD(GameThreadSTD && other) noexcept;
		GameThreadSTD & operator=(GameThreadSTD && other) noexcept;

	private:
		std::thread t_;
	};
}
