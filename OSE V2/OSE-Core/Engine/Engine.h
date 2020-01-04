#pragma once
#include "EngineTask.h"

namespace ose
{
	// abstract engine class
	// all engines (rendering, physics, animation, etc.) extend this class
	// allows user defined engines to be created
	// templated by the type of pool on which the engine acts
	///template <class T>
	class Engine
	{
	protected:
		Engine() {}
	public:
		~Engine() {}

		/// called every game update for every data object in pool
		/// IMPORTANT - stub method should not be called...
		/// ...used to allow update method with DataObject sub-class to be called
		// Processes all elements in the pool then returns
		///virtual void update(EnginePool & pool) {}

		// defaults to false
		// override in subclass if the engine is multithreadable (threadsafe)
		// note - setting to true for custom components with limited use may reduce performance & increase memory usage
		static const bool multithreadable;
	};

	const bool multithreadable = false;
}
