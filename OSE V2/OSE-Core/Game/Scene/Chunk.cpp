#include "stdafx.h"
#include "Chunk.h"
#include "OSE-Core/Game/Game.h"

namespace ose
{

	Chunk::Chunk(const std::string & name) : EntityList(), name_(name)
	{
	
	}

	Chunk::~Chunk()
	{
	
	}

	void Chunk::SetEnabled(bool a)
	{
		enabled_ = a;
		if(game_ && a)
			game_->OnEntityActivated(*this);
		else if(game_ && !a)
			game_->OnEntityDeactivated(*this);
	}

	void Chunk::Enable()
	{
		enabled_ = true;
		if(game_)
			game_->OnEntityActivated(*this);
	}

	void Chunk::Disable()
	{
		enabled_ = false;
		if(game_)
			game_->OnEntityDeactivated(*this);
	}
}
