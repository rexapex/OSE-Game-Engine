#pragma once

#include "../../stdafx.h"

namespace origami_sheep_engine
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
