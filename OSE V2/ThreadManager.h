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

		//called by threads to get a new task
		void getNewTask();
		
	private:
		//array of threads which will carry out expensive processing
		std::vector<std::unique_ptr<GameThread>> threads_;

		//for testing purposes let tasks by strings to print
		std::vector<std::string> tasks_;

		//create the array of threads
		//function decides how many threads are needed
		void createThreads();
	};
}
