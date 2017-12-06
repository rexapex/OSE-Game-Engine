#pragma once

#include <thread>
#include "stdafx.h"

namespace origami_sheep_engine
{
	class GameThread
	{
	public:
		GameThread();
		virtual ~GameThread() noexcept;

		//copy constructors
		GameThread(const GameThread &) = delete;
		GameThread & operator=(const GameThread &) = delete;

		//move constructors
		GameThread(GameThread && other) noexcept = default;
		GameThread & operator=(GameThread && other) noexcept = default;

	protected:
		void run();
	};
}
