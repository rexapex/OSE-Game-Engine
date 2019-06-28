#pragma once

#include <mutex>
#include "GameThread.h"

namespace ose::game
{
	class ThreadManager
	{
	public:
		ThreadManager(const RenderPool & render_pool);
		~ThreadManager() noexcept;
		ThreadManager(ThreadManager && other) noexcept;
		ThreadManager & operator=(ThreadManager && other) noexcept = delete;

		void addNewTask(const std::string &);

		// process all rendering tasks
		void processRenderTasks();

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

		// reference to the pool of render tasks
		const RenderPool & render_pool_;
	};
}
