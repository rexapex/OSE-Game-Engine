#include "stdafx.h"
#include "GameThread.h"

namespace origami_sheep_engine
{
	GameThread::GameThread(const std::string & name, std::function<void(std::string &)> get_new_task, std::mutex & mu, std::condition_variable & work_to_do)
		: name_(name), get_new_task_(get_new_task), mu_(mu), work_to_do_(work_to_do) {}

	GameThread::~GameThread() {}

	//move constructors
	GameThread::GameThread(GameThread && other) noexcept
		: name_(std::move(other.name_)), get_new_task_(std::move(other.get_new_task_)), mu_(std::move(other.mu_)), work_to_do_(std::move(other.work_to_do_)) {}

	GameThread & GameThread::operator=(GameThread && other) noexcept
	{
		this->name_ = std::move(other.name_);
		this->get_new_task_ = std::move(other.get_new_task_);
		return *this;
	}

	void GameThread::run()
	{
		DEBUG_LOG("thread started");
		//std::string task;
		//thread_manager_.getNewTask(task);
		while(true)
		{
			{
				std::unique_lock<std::mutex> lock(mu_);
				work_to_do_.wait(lock, [this] {
					std::string task;
					get_new_task_(task);
					if(task != "")
						LOG(task << "," << name_);
					return task != "";
				});
			}
			//LOG("update");
		}
		//on_task_completed_(*this);
	}
}
