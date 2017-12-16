#include "stdafx.h"
#include "GameThreadSTD.h"

namespace origami_sheep_engine
{
	GameThreadSTD::GameThreadSTD(const std::string & name, std::function<void(std::string &)> get_new_task_, std::mutex & mu, std::condition_variable & work_to_do)
		: GameThread::GameThread(name, get_new_task_, mu, work_to_do)
	{
		this->t_ = std::thread(&GameThreadSTD::run, this);
	}

	GameThreadSTD::~GameThreadSTD()
	{
		DEBUG_LOG("DESTRUCTING");
		this->t_.join();
	}

	//move constructors
	GameThreadSTD::GameThreadSTD(GameThreadSTD && other) noexcept : GameThread::GameThread(std::move(other))
	{
		this->t_ = std::move(other.t_);
	}

	GameThreadSTD & GameThreadSTD::operator=(GameThreadSTD && other) noexcept
	{
		this->t_ = std::move(other.t_);
		return *this;
	}
}
