#pragma once

#include <mutex>
#include "GameThread.h"
#include "OSE-Core/Rendering/RenderPool.h"

// IMPORTANT - ThreadManager & GameThread currently used
// Switched to using OpenMP to simplify pooling and multi-threading code

namespace ose
{
	class ThreadManager
	{
	public:
		ThreadManager(RenderPool const & render_pool);
		~ThreadManager() noexcept;
		ThreadManager(ThreadManager && other) noexcept;
		ThreadManager & operator=(ThreadManager && other) noexcept = delete;

		void AddNewTask(std::string const &);

		// process all rendering tasks
		void ProcessRenderTasks();

	private:
		uint32_t tasks_in_progress_;

		//array of threads which will carry out expensive processing
		std::vector<uptr<GameThread>> threads_;

		//for testing purposes let tasks by strings to print
		std::vector<std::string> tasks_;

		//create the array of threads
		//function decides how many threads are needed
		void CreateThreads();

		void GetNewTask(std::string & task);

		void OnTaskCompleted(uint32_t thread_id);

		std::mutex mu_;
		std::condition_variable work_to_do_;

		// reference to the pool of render tasks
		RenderPool const & render_pool_;
	};
}
