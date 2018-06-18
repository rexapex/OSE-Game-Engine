#pragma once
#include "DataObject.h"

namespace ose::engine
{
	// abstract engine class
	// all engines (rendering, physics, animation, etc.) extend this class
	// allows user defined engines to be created
	///template <typename DataObjectType>
	class Engine
	{
	protected:
		Engine() {}
	public:
		~Engine() {}

		// called every game update for every data object in pool
		void update(DataObject &) {}
		//virtual void update() = 0;

		// defaults to false
		// override in subclass if the engine is multithreadable (threadsafe)
		// note - setting to true for custom components with limited use may reduce performance & increase memory usage
		static const bool multithreadable;
	};

	const bool multithreadable = false;
}
