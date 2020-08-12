#pragma once

namespace ose
{
	class Entity;

	// Agent's distance from chunks determines when chunks are loaded/unloaded
	struct ChunkManagerSettings
	{
		float load_distance_			{ 0 };
		float unload_distance_			{ 0 };
		unowned_ptr<Entity> agent_		{ nullptr };
	};
}
