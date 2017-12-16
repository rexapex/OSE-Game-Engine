#include "stdafx.h"
#include "ThreadManager.h"

namespace origami_sheep_engine
{
	ThreadManager::ThreadManager()
	{
		//only need to call in regular constructor
		createThreads();
		addNewTask("task1");
		addNewTask("task2");
		addNewTask("task3");
		addNewTask("task4");
		addNewTask("task5");
		addNewTask("task6");
		addNewTask("task7");
	}

	ThreadManager::~ThreadManager() noexcept {}

	ThreadManager::ThreadManager(ThreadManager && other) noexcept : threads_(std::move(other.threads_)) {}

	ThreadManager & ThreadManager::operator=(ThreadManager && other) noexcept
	{
		this->threads_ = std::move(other.threads_);
		return *this;
	}


	//create the array of threads
	//function decides how many threads are needed
	void ThreadManager::createThreads()
	{
		//set the number of threads to the number of CPU cores
		size_t num_threads = std::thread::hardware_concurrency() == 0 ? 1 : std::thread::hardware_concurrency();

		//avoids unnecessary resizing later
		threads_.reserve(num_threads);

		auto callback = [this] (std::string & task) {
			//mu.lock();
			this->getNewTask(task);
			//mu.unlock();
		};

		//create the threads
		for(size_t t = 0; t < num_threads; t++)
		{
			threads_.emplace_back(std::make_unique<GameThreadImpl>(std::to_string(t), callback, mu_, work_to_do_));
		}

		DEBUG_LOG("created " << num_threads << " threads");
	}


	//called by a thread upon completing its task
	void ThreadManager::getNewTask(std::string & task)
	{
		//DEBUG_LOG(thread.name_);
		if(tasks_.size() > 0)
		{
			//give the thread a new task if one exists
			task.assign(tasks_.back());
			tasks_.pop_back();
		}
		else
		{
			task.assign("");
		}
	}


	void ThreadManager::addNewTask(const std::string & t)
	{
		{
			std::unique_lock<std::mutex> lock(mu_);
			tasks_.push_back(t);
			LOG("added " << t);
		}
		work_to_do_.notify_one();
	}
}
