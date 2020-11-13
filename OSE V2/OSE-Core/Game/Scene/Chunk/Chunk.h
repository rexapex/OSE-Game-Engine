#pragma once
#include "OSE-Core/Entity/EntityList.h"
#include "OSE-Core/Math/Transformable.h"

namespace ose
{
	class Game;
	class Project;
	class ProjectLoader;

	class Chunk final : public EntityList
	{
	public:
		Chunk(std::string const & name, std::string const & path, Project const & project, ProjectLoader & project_loader);
		virtual ~Chunk() noexcept;
		Chunk(Chunk const & other);
		Chunk(Chunk &&) noexcept = default;
		Chunk & operator=(Chunk &) = delete;
		Chunk & operator=(Chunk &&) = delete;

		// Load the chunk from the filesystem
		void Load();

		// Clear the list of entities and return chunk to unloaded state
		void Unload();

		void SetName(std::string const & name) { name_ = name; }
		std::string const & GetName() const { return name_; }

		std::string const & GetPath() const { return path_; }

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
