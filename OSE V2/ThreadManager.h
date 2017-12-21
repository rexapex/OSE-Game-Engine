#pragma once

#include <mutex>
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

		void addNewTask(const std::string &);

	private:
		uint32_t tasks_in_progress_;

		//array of threads which will carry out expensive processing
		std::vector<std::unique_ptr<GameThread>> threads_;

		//for testing purposes let tasks by strings to print
		std::vector<std::string> tasks_;

		//create the array of threads
		//function decides how many threads are needed
		void createThreads();

		void getNewTask(std::string & task);

		void onTaskCompleted(uint32_t thread_id);

		std::mutex mu_;
		std::condition_variable work_to_do_;
	};
}
