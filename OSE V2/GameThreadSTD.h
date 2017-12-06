#pragma once

#include <thread>
#include "stdafx.h"
#include "GameThread.h"

namespace origami_sheep_engine
{
	class GameThreadSTD : public GameThread
	{
	public:
		GameThreadSTD();
		~GameThreadSTD() noexcept;

		//copy constructors
		GameThreadSTD(const GameThreadSTD &) = delete;
		GameThreadSTD & operator=(const GameThreadSTD &) = delete;

		//move constructors
		GameThreadSTD(GameThreadSTD && other) noexcept;
		GameThreadSTD & operator=(GameThreadSTD && other) noexcept;

	private:
		std::thread t_;
	};
}
