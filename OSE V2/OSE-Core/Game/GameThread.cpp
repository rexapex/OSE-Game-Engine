#include "../../stdafx.h"
#include "GameThread.h"

namespace origami_sheep_engine
{
	GameThread::GameThread(uint32_t thread_id, std::function<void(std::string &)> get_new_task, std::mutex & mu,
							std::condition_variable & work_to_do, std::function<void(uint32_t thread_id)> on_task_completed)
		: thread_id_(thread_id), get_new_task_(get_new_task), mu_(mu), work_to_do_(work_to_do), on_task_completed_(on_task_completed)
	{
		this->t_ = std::thread(&GameThread::run, this);
	}

	GameThread::~GameThread()
	{
		this->t_.join();
	}

	//move constructors
	GameThread::GameThread(GameThread && other) noexcept
		: thread_id_(std::move(other.thread_id_)), get_new_task_(std::move(other.get_new_task_)),
		mu_(std::move(other.mu_)), work_to_do_(std::move(other.work_to_do_)), t_(std::move(other.t_)),
		on_task_completed_(std::move(other.on_task_completed_)) {}

	GameThread & GameThread::operator=(GameThread && other) noexcept
	{
		this->thread_id_ = std::move(other.thread_id_);
		this->get_new_task_ = std::move(other.get_new_task_);
		this->on_task_completed_ = std::move(other.on_task_completed_);
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
			LOG(task << "," << thread_id_);

			//notify ThreadManager that task has been completed ?
			on_task_completed_(thread_id_);
		}
	}
}
