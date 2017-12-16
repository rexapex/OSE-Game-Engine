#pragma once

#include <thread>
#include <functional>
#include <mutex>
#include "stdafx.h"

namespace origami_sheep_engine
{
	class GameThread
	{
	public:
		GameThread(const std::string & name, std::function<void(std::string &)> get_new_task, std::mutex & mu, std::condition_variable & work_to_do);
		virtual ~GameThread() noexcept;

		//copy constructors
		GameThread(const GameThread &) = delete;
		GameThread & operator=(const GameThread &) = delete;

		//move constructors
		GameThread(GameThread && other) noexcept;
		GameThread & operator=(GameThread && other) noexcept;

		std::string name_;

	private:
		void run();

		std::thread t_;

		std::function<void(std::string &)> get_new_task_;

		std::mutex & mu_;

		std::condition_variable & work_to_do_;
	};
}
