#pragma once

namespace ose
{
	class Entity;

	// Agent's distance from chunks determines when chunks are loaded/unloaded
	struct ChunkManagerSettings
	{
		float load_distance_			{ 100.0f };
		float unload_distance_			{ 250.0f };
		unowned_ptr<Entity> agent_		{ nullptr };
	};
}
