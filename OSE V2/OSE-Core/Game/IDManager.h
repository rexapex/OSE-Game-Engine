#pragma once

namespace ose::game
{
	typedef uint32_t EntityID;	// NOTE - Might change this to uint64_t later

	class IDManager
	{
	public:
		// Get the next available entity ID
		static EntityID NextEntityId()
		{
			static EntityID id { 0 };
			return id++;
		}

	private:
		// Private constructor so class can't be instantiated
		IDManager() {}
	};
}
