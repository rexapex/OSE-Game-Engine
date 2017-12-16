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
		GameThread(uint32_t thread_id, std::function<void(std::string &)> get_new_task, std::mutex & mu,
					std::condition_variable & work_to_do, std::function<void(uint32_t thread_id)> on_task_completed);
		virtual ~GameThread() noexcept;

		//copy constructors
		GameThread(const GameThread &) = delete;
		GameThread & operator=(const GameThread &) = delete;

		//move constructors
		GameThread(GameThread && other) noexcept;
		GameThread & operator=(GameThread && other) noexcept;

	private:
		void run();

		std::thread t_;

		uint32_t thread_id_;

		std::function<void(std::string &)> get_new_task_;

		std::function<void(uint32_t thread_id)> on_task_completed_;

		std::mutex & mu_;

		std::condition_variable & work_to_do_;
	};
}
