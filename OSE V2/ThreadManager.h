#pragma once

#include "GameThread.h"

namespace origami_sheep_engine
{
	class ThreadManager
	{
	public:
		ThreadManager();
		~ThreadManager() noexcept;
		ThreadManager(ThreadManager && other) noexcept;
		ThreadManager & operator=(ThreadManager && other) noexcept;
		
	private:
		//array of threads which will carry out expensive processing
		std::vector<GameThread> threads_;

		//create the array of threads
		//function decides how many threads are needed
		void createThreads();
	};
}
