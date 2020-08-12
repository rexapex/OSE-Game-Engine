#pragma once
#include "ChunkManagerSettings.h"

namespace ose
{
	class Chunk;

	class ChunkManager
	{
	public:
		ChunkManager();
		virtual ~ChunkManager() noexcept;
		ChunkManager(ChunkManager const &) noexcept;
		ChunkManager & operator=(ChunkManager &) = delete;

		// Determine whether chunks should be loaded/unloaded
		void UpdateChunks();

		// Set the chunk manager settings
		void SetSettings(ChunkManagerSettings settings) { settings_ = settings; }

	private:
		// Chunks split into 2 lists, loaded and unloaded
		std::vector<std::unique_ptr<Chunk>> loaded_chunks_;
		std::vector<std::unique_ptr<Chunk>> unloaded_chunks_;

		// Settings determine when chunks are loaded/unloaded
		ChunkManagerSettings settings_;
	};
}
