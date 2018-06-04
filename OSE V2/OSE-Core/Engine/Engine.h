#pragma once

namespace ose::engine
{
	// abstract engine class
	// all engines (rendering, physics, animation, etc.) extend this class
	// allows user defined engines to be created
	class Engine
	{
	protected:
		Engine() {}
	public:
		~Engine() {}

		// called every game update
		virtual void update() = 0;
		
		// defaults to false
		// override in subclass if the engine is multithreadable (threadsafe)
		// note - setting to true for custom components with limited use may reduce performance & increase memory usage
		static const bool multithreadable;
	};

	const bool multithreadable = false;
}
