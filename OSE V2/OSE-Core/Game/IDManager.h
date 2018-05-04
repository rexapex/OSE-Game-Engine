#pragma once

namespace ose::game
{
	class IDManager
	{
	public:
		//get the next available entity ID
		static uint32_t next_entity_ID()
		{
			static uint32_t id = 0;
			return id++;
		}

	private:
		//private constructor so class can't be instantiated
		IDManager() {}
	};
}
