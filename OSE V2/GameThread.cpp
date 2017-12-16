#include "stdafx.h"
#include "GameThread.h"

namespace origami_sheep_engine
{
	GameThread::GameThread(const std::string & name, std::function<void(std::string &)> get_new_task, std::mutex & mu, std::condition_variable & work_to_do)
		: name_(name), get_new_task_(get_new_task), mu_(mu), work_to_do_(work_to_do)
	{
		this->t_ = std::thread(&GameThreadSTD::run, this);
	}

	GameThread::~GameThread()
	{
		this->t_.join();
	}

	//move constructors
	GameThread::GameThread(GameThread && other) noexcept
		: name_(std::move(other.name_)), get_new_task_(std::move(other.get_new_task_)), mu_(std::move(other.mu_)), work_to_do_(std::move(other.work_to_do_))
	{
		this->t_ = std::move(other.t_);
	}

	GameThread & GameThread::operator=(GameThread && other) noexcept
	{
		this->name_ = std::move(other.name_);
		this->get_new_task_ = std::move(other.get_new_task_);
		this->t_ = std::move(other.t_);
		return *this;
	}

	void GameThread::run()
	{
		DEBUG_LOG("thread started");
		while(true)
		{
			std::string task;
			{
				std::unique_lock<std::mutex> lock(mu_);
				//wait until a new task becomes available
				work_to_do_.wait(lock, [this, &task] {
					get_new_task_(task);
					return task != "";
				});
			}

			//perform the task
			LOG(task << "," << name_);

			//notify ThreadManager that task has been completed ?
		}
	}
}
