#include "stdafx.h"
#include "ChunkManager.h"
#include "Chunk.h"
#include "OSE-Core/Entity/Entity.h"
#include "OSE-Core/Project/ProjectLoader.h"
#include "OSE-Core/Math/ITransform.h"
#include "OSE-Core/Game/Game.h"

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
		settings_ = other.settings_;
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
		if(agent_)
		{
			for(auto & iter = unloaded_chunks_.begin(); iter != unloaded_chunks_.end();)
			{
				std::unique_ptr<Chunk> & chunk = *iter;
				if(glm::distance2(chunk->GetGlobalTransform().GetTranslation(), agent_->GetGlobalTransform().GetTranslation()) <= std::pow(settings_.load_distance_, 2))
				{
					chunk->Load();
					OnChunkActivated(*chunk);
					std::move(iter, iter+1, std::back_inserter(loaded_chunks_));
					iter = unloaded_chunks_.erase(iter);
				}
				else
				{
					++iter;
				}
			}

			for(auto & iter = loaded_chunks_.begin(); iter != loaded_chunks_.end();)
			{
				std::unique_ptr<Chunk> & chunk = *iter;
				if(glm::distance2(chunk->GetGlobalTransform().GetTranslation(), agent_->GetGlobalTransform().GetTranslation()) >= std::pow(settings_.unload_distance_, 2))
				{
					OnChunkDeactivated(*chunk);
					chunk->Unload();
					std::move(iter, iter+1, std::back_inserter(unloaded_chunks_));
					iter = loaded_chunks_.erase(iter);
				}
				else
				{
					++iter;
				}
			}
		}
	}

	// Reset the chunk manager agent, e.g. find the agent entity from the entities of the game
	void ChunkManager::ResetChunkManagerAgent(Game * game)
	{
		if(!settings_.agent_name_.empty())
		{
			std::vector<Entity *> entities = game->FindAllEntitiesWithName(settings_.agent_name_);
			if(!entities.empty())
				agent_ = entities[0];
		}
	}

	// Find all the entities within loaded chunks with the given name
	std::vector<Entity *> ChunkManager::FindLoadedChunkEntitiesWithName(std::string_view name) const
	{
		std::vector<Entity *> vec;
		FindLoadedChunkEntitiesWithName(name, vec);
		return vec;
	}

	// Find all the entities within loaded chunks with the given name and add them to the vector passed
	void ChunkManager::FindLoadedChunkEntitiesWithName(std::string_view name, std::vector<Entity *> out_vec) const
	{
		for(auto & chunk : loaded_chunks_)
		{
			chunk->FindDescendentEntitiesWithName(name, out_vec);
		}
	}
}
