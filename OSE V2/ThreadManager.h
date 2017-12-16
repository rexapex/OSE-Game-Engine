#pragma once

#include <mutex>
#include "GameThread.h"

//fixes cyclic dependency issue
//class GameThread;

namespace origami_sheep_engine
{
	class ThreadManager
	{
	public:
		ThreadManager();
		~ThreadManager() noexcept;
		ThreadManager(ThreadManager && other) noexcept;
		ThreadManager & operator=(ThreadManager && other) noexcept;

		void addNewTask(const std::string &);

	private:
		//array of threads which will carry out expensive processing
		std::vector<std::unique_ptr<GameThread>> threads_;

		//for testing purposes let tasks by strings to print
		std::vector<std::string> tasks_;

		//create the array of threads
		//function decides how many threads are needed
		void createThreads();

		void getNewTask(std::string & task);

		std::mutex mu_;
		std::condition_variable work_to_do_;
	};
}
