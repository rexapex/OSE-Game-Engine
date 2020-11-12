#pragma once
#include "ChunkManagerSettings.h"

namespace ose
{
	class ProjectLoader;
	class SceneManager;
	class Chunk;

	class ChunkManager
	{
	public:
		ChunkManager();
		virtual ~ChunkManager() noexcept;
		ChunkManager(ChunkManager const &);
		ChunkManager & operator=(ChunkManager &) = delete;

		// Add a chunk to the unloaded chunks list
		// Method constructs a new object
		// Method takes chunk constructor arguments
		// Returns a reference to the newly created chunk
		template<typename... Args>
		unowned_ptr<Chunk> AddChunk(Args &&... params)
		{
			// Construct a new chunk object
			try {
				return unloaded_chunks_.emplace_back( std::make_unique<Chunk>(std::forward<Args>(params)...) ).get();
			} catch(...) {
				return nullptr;
			}
		}

		// Determine whether chunks should be loaded/unloaded
		void UpdateChunks();

		// Apply the settings to the chunk manager
		void ApplyChunkManagerSettings(ChunkManagerSettings settings) { settings_ = settings; }

		// Find all the entities within loaded chunks with the given name
		std::vector<Entity *> FindLoadedChunkEntitiesWithName(std::string_view name);

	protected:
		virtual void OnChunkActivated(Chunk & chunk) = 0;
		virtual void OnChunkDeactivated(Chunk & chunk) = 0;

	private:
		// Chunks split into 2 lists, loaded and unloaded
		std::vector<std::unique_ptr<Chunk>> loaded_chunks_;
		std::vector<std::unique_ptr<Chunk>> unloaded_chunks_;

		// Settings determine when chunks are loaded/unloaded
		ChunkManagerSettings settings_;
	};
}
