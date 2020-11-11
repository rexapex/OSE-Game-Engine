#include "stdafx.h"
#include "Chunk.h"
#include "OSE-Core/Entity/Entity.h"
#include "OSE-Core/Game/Game.h"
#include "OSE-Core/Project/ProjectLoader.h"

namespace ose
{

	Chunk::Chunk(const std::string & name, const std::string & path, Project const & project, ProjectLoader & project_loader) : EntityList(nullptr), name_(name), path_(path), project_(project), project_loader_(project_loader)
	{
	
	}

	Chunk::~Chunk()
	{
	
	}

	Chunk::Chunk(const Chunk & other) : EntityList(nullptr),
		name_(other.name_), path_(other.path_), project_(other.project_), project_loader_(other.project_loader_)
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
