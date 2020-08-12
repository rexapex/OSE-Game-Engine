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
