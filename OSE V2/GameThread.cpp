#include "stdafx.h"
#include "GameThread.h"

namespace origami_sheep_engine
{
	GameThread::GameThread() {}

	GameThread::~GameThread() {}

	void GameThread::run()
	{
		while(true)
		{
			LOG("update");
		}
	}
}
