#include "stdafx.h"
#include "Chunk.h"
#include "OSE-Core/Entity/Entity.h"
#include "OSE-Core/Game/Game.h"
#include "OSE-Core/Project/ProjectLoader.h"

namespace ose
{

	Chunk::Chunk(const std::string & name, const std::string & path, Project const & project, ProjectLoader & project_loader) : EntityList(), Transformable(), name_(name), path_(path), project_(project), project_loader_(project_loader)
	{
	
	}

	Chunk::~Chunk()
	{
	
	}

	// Load the chunk from the filesystem
	void Chunk::Load()
	{
		project_loader_.LoadChunk(*this, project_);
	}

	// Clear the list of entities and return chunk to unloaded state
	void Chunk::Unload()
	{
		entities_.clear();
	}
}
