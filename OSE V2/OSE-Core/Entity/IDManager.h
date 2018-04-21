#pragma once

namespace ose::entity
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
	};
}
