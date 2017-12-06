#include "stdafx.h"
#include "GameThreadSTD.h"

namespace origami_sheep_engine
{
	GameThreadSTD::GameThreadSTD()
	{
		this->t_ = std::thread(&GameThreadSTD::run, this);
	}

	GameThreadSTD::~GameThreadSTD() {}

	//move constructors
	GameThreadSTD::GameThreadSTD(GameThreadSTD && other) noexcept
	{
		this->t_ = std::move(other.t_);
	}

	GameThreadSTD & GameThreadSTD::operator=(GameThreadSTD && other) noexcept
	{
		this->t_ = std::move(other.t_);
		return *this;
	}

}
