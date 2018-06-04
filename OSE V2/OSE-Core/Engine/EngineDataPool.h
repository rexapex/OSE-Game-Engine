#pragma once
#include "OSE-Core/Rendering/RenderObject.h"

namespace ose::engine
{
	// each engine has an associated data pool containing all objects to process
	// e.g. for RenderPool, DataObject will be replaced with RenderObject
	// template based approach avoids the needs for dynamic_cast or any_cast on add and remove methods...
	// ...also prevents writers of scripts from having to implement their own data pool subclass
	template <typename DataObject>
	class EngineDataPool
	{
	protected:
		EngineDataPool() {}
	public:
		~EngineDataPool() {}

		// add an engine data object to the data pool
		// called when a component is loaded, or when a component is re-enabled
		void addEngineDataObject(const DataObject & obj) = 0;

		// remove an engine data object to the data pool
		// called when a component/entity is removed, or when a component is disabled
		void removeEngineDataObject(const DataObject & obj) = 0;

	private:
		// list of data objects relevant to the corresponding engine
		std::vector<DataObject> data_objects_;
	};

	// define the base types of engine data pool core to the engine
	typedef EngineDataPool<RenderObject> RenderPool;
}
