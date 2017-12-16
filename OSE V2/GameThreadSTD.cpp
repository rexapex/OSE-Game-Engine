#include "stdafx.h"
#include "GameThreadSTD.h"

namespace origami_sheep_engine
{
	GameThreadSTD::GameThreadSTD(const std::string & name, std::function<void(std::string &)> get_new_task_, std::mutex & mu, std::condition_variable & work_to_do)
		: GameThread::GameThread(name, get_new_task_, mu, work_to_do)
	{
	}

	GameThreadSTD::~GameThreadSTD()
	{
		DEBUG_LOG("DESTRUCTING");
	}

	//move constructors
	GameThreadSTD::GameThreadSTD(GameThreadSTD && other) noexcept : GameThread::GameThread(std::move(other))
	{
	}

	GameThreadSTD & GameThreadSTD::operator=(GameThreadSTD && other) noexcept
	{
		return *this;
	}
}
