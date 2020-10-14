#pragma once
#include "OSE-Core/Entity/EntityList.h"
#include "OSE-Core/Math/Transformable.h"

namespace ose
{
	class Game;
	class Project;
	class ProjectLoader;

	class Chunk final : public EntityList, public Transformable<std::unique_ptr<Entity>>
	{
	public:
		Chunk(const std::string & name, const std::string & path, Project const & project, ProjectLoader & project_loader);
		virtual ~Chunk() noexcept;
		Chunk(const Chunk & other) = default;
		Chunk(Chunk &&) noexcept = default;
		Chunk & operator=(Chunk &) = delete;
		Chunk & operator=(Chunk &&) = delete;

		// Load the chunk from the filesystem
		void Load();

		// Clear the list of entities and return chunk to unloaded state
		void Unload();

		void SetName(const std::string & name) { this->name_ = name; }
		const std::string & GetName() const { return this->name_; }

		const std::string & GetPath() const { return this->path_; }

		// Get a list of transformable elements
		// Returns a list of child entities
		virtual const std::vector<std::unique_ptr<Entity>> & GetChildTransformables() override { return entities_; }

	private:
		// Name of the chunk file (excluding the file extension)
		std::string name_;

		// Absolute path of the chunk file (computed by project loader)
		std::string path_;

		// Project and project loader of the game (scene manager) this chunk belongs to
		// Required for loading the chunk and its resources at arbitrary point during execution
		Project const & project_;
		ProjectLoader & project_loader_;
	};
}
