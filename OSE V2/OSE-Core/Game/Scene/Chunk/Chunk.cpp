#include "stdafx.h"
#include "Chunk.h"
#include "OSE-Core/Entity/Entity.h"
#include "OSE-Core/Game/Game.h"

namespace ose
{

	Chunk::Chunk(const std::string & name, const std::string & path) : EntityList(), Transformable(), name_(name), path_(path)
	{
	
	}

	Chunk::~Chunk()
	{
	
	}

	// Clear the list of entities and return chunk to unloaded state
	void Chunk::Unload()
	{
		entities_.clear();
	}
}
