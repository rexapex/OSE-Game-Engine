#include "stdafx.h"
#include "ChunkManager.h"
#include "Chunk.h"
#include "OSE-Core/Entity/Entity.h"

namespace ose
{
	ChunkManager::ChunkManager() : settings_()
	{
	
	}

	ChunkManager::~ChunkManager()
	{
	
	}

	ChunkManager::ChunkManager(ChunkManager const & other)
	{
		// Perform a deep copy of all chunks
		loaded_chunks_.clear();
		unloaded_chunks_.clear();
		for(auto const & c : other.loaded_chunks_)
			loaded_chunks_.push_back(std::make_unique<Chunk>(*c));
		for(auto const & c : other.unloaded_chunks_)
			unloaded_chunks_.push_back(std::make_unique<Chunk>(*c));
	}
	
	// Determine whether chunks should be loaded/unloaded
	void ChunkManager::UpdateChunks()
	{
		if(settings_.agent_)
		{
			for(auto & chunk : unloaded_chunks_)
			{
				if(glm::distance2(chunk->GetGlobalTransform().GetPosition(), settings_.agent_->GetGlobalTransform().GetPosition()) <= std::pow(settings_.load_distance_, 2))
					;//LOAD
			}

			for(auto & chunk : loaded_chunks_)
			{
				if(glm::distance2(chunk->GetGlobalTransform().GetPosition(), settings_.agent_->GetGlobalTransform().GetPosition()) >= std::pow(settings_.unload_distance_, 2))
					;//UNLOAD
			}
		}
	}
}
