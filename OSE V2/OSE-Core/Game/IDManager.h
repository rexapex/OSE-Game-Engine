#pragma once

namespace ose::game
{
	typedef uint32_t EntityID;	// might change this to uint64_t later

	class IDManager
	{
	public:
		//get the next available entity ID
		static EntityID next_entity_ID()
		{
			static EntityID id = 0;
			return id++;
		}

	private:
		//private constructor so class can't be instantiated
		IDManager() {}
	};
}
