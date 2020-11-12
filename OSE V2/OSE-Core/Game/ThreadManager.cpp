#include "stdafx.h"
#include "ThreadManager.h"
#include "OSE-Core/Rendering/RenderPool.h"

namespace ose
{
	ThreadManager::ThreadManager(RenderPool const & render_pool) : render_pool_(render_pool)
	{
		//only need to call in regular constructor
		CreateThreads();
	}

	ThreadManager::~ThreadManager() noexcept {}

	ThreadManager::ThreadManager(ThreadManager && other) noexcept : threads_(std::move(other.threads_)),
		tasks_in_progress_(std::move(other.tasks_in_progress_)), render_pool_(std::move(other.render_pool_)) {}

	/*ThreadManager & ThreadManager::operator=(ThreadManager && other) noexcept
	{
		this->threads_ = std::move(other.threads_);
		this->tasks_in_progress_ = std::move(other.tasks_in_progress_);
		this->render_pool_ = other.render_pool_;
		return *this;
	}*/


	//create the array of threads
	//function decides how many threads are needed
	void ThreadManager::CreateThreads()
	{
		//set the number of threads to the number of CPU cores
		uint32_t num_threads {std::thread::hardware_concurrency() == 0 ? 1 : std::thread::hardware_concurrency()};

		//avoids unnecessary resizing later
		threads_.reserve(num_threads);

		auto get_new_task = [this] (std::string & task) {
			this->GetNewTask(task);
		};

		auto on_task_completed = [this] (uint32_t thread_id) {
			this->OnTaskCompleted(thread_id);
		};

		tasks_in_progress_ = 0;

		//create the threads
		for(uint32_t t = 1; t <= num_threads; t++)
		{
			threads_.emplace_back(ose::make_unique<GameThread>(t, get_new_task, mu_, work_to_do_, on_task_completed));
		}

		DEBUG_LOG("created", num_threads, "threads");
	}


	//called by a thread upon completing its task
	void ThreadManager::GetNewTask(std::string & task)
	{
		//DEBUG_LOG(thread.name_);
		if(tasks_.size() > 0)
		{
			//give the thread a new task if one exists
			tasks_in_progress_ ++;
			task.assign(tasks_.back());
			tasks_.pop_back();
		}
		else
		{
			task.assign("");
		}
	}


	void ThreadManager::AddNewTask(const std::string & t)
	{
		{
			std::unique_lock<std::mutex> lock(mu_);
			tasks_.push_back(t);
			LOG("Added New Task:", t);
		}
		work_to_do_.notify_one();
	}


	void ThreadManager::OnTaskCompleted(uint32_t thread_id)
	{
		std::unique_lock<std::mutex> lock(mu_);
		tasks_in_progress_ --;
		if(tasks_in_progress_ == 0 && tasks_.size() == 0)
		{
			LOG("All tasks complete\n");
		}
	}

	// process all rendering tasks
	void ThreadManager::ProcessRenderTasks()
	{
		// Render the game
		/*RenderTaskImpl * render_object = nullptr;
		while((render_object = render_pool_.GetNextDataObject()) != nullptr)
		{
			render_object->Update();
		///	rendering_engine_->update(*render_object);
		}*/
	}
}
