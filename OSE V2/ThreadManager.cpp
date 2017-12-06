#include "stdafx.h"
#include "ThreadManager.h"

void runthread() {}

namespace origami_sheep_engine
{
	ThreadManager::ThreadManager()
	{
		//only need to call in regular constructor
		createThreads();
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

		//create the threads
		for(size_t t = 0; t < num_threads; t++)
		{
			threads_.emplace_back(GameThreadImpl());
		}
	}
}
