#include "stdafx.h"
#include "ChunkManager.h"
#include "Chunk.h"
#include "OSE-Core/Entity/Entity.h"
#include "OSE-Core/Project/ProjectLoader.h"

namespace ose
{
	ChunkManager::ChunkManager(ProjectLoader * project_loader) : settings_(), project_loader_(project_loader)
	{
	
	}

	ChunkManager::~ChunkManager()
	{
	
	}

	ChunkManager::ChunkManager(ChunkManager const & other)
	{
		settings_ = other.settings_;
		project_loader_ = other.project_loader_;
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
			for(auto & iter = unloaded_chunks_.begin(); iter != unloaded_chunks_.end();)
			{
				std::unique_ptr<Chunk> & chunk = *iter;
				if(glm::distance2(chunk->GetGlobalTransform().GetPosition(), settings_.agent_->GetGlobalTransform().GetPosition()) <= std::pow(settings_.load_distance_, 2))
				{
					project_loader_->LoadChunk(*chunk);
					unloaded_chunks_.erase(iter);
					loaded_chunks_.push_back(std::move(chunk));
					OnChunkActivated(*chunk);
				}
				else
				{
					++iter;
				}
			}

			for(auto & iter = loaded_chunks_.begin(); iter != loaded_chunks_.end();)
			{
				std::unique_ptr<Chunk> & chunk = *iter;
				if(glm::distance2(chunk->GetGlobalTransform().GetPosition(), settings_.agent_->GetGlobalTransform().GetPosition()) >= std::pow(settings_.unload_distance_, 2))
				{
					chunk->Unload();
					loaded_chunks_.erase(iter);
					unloaded_chunks_.push_back(std::move(chunk));
					OnChunkDeactivated(*chunk);
				}
				else
				{
					++iter;
				}
			}
		}
	}
}
