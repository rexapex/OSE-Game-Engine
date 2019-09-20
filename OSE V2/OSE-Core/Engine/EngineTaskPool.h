#pragma once
#include <algorithm>

namespace ose::engine
{
	// each engine has an associated data pool containing all objects to process
	// e.g. for RenderPool, DataObject will be replaced with RenderObject
	// template based approach avoids the needs for dynamic_cast or any_cast on add and remove methods...
	// ...also prevents writers of scripts from having to implement their own data pool subclass
	template <typename DataObject>
	class EngineTaskPool
	{
	public:
		EngineTaskPool() {}
		~EngineTaskPool() {}
		EngineTaskPool(EngineTaskPool &) = delete;
		EngineTaskPool & operator=(EngineTaskPool &) = delete;

		// copy constructor
		EngineTaskPool(EngineTaskPool && other) : data_objects_(std::move(other.data_objects_)) {}

		// copy assignment constructor
		EngineTaskPool & operator=(EngineTaskPool && other)
		{
			this->data_objects_ = std::move(other.data_objects_);
			return *this;
		}

		// add an engine data object to the data pool
		// called when a component is loaded, or when a component is re-enabled
		void AddEngineDataObject(DataObject * obj)
		{
			data_objects_.push_back(obj);
		}

		// remove an engine data object to the data pool
		// called when a component/entity is removed, or when a component is disabled
		void RemoveEngineDataObject(DataObject * obj)
		{
			data_objects_.erase(std::remove(data_objects_.begin(), data_objects_.end(), obj), data_objects_.end());
		}

		// get the next data object
		DataObject * GetNextDataObject() const
		{
			static int next_index = 0;
			if(next_index < data_objects_.size()) {
				// return the object at next_index
				auto & next = data_objects_[next_index];
				next_index ++;
				return next;
			} else {
				// return nullptr once the end is reached
				next_index = 0;
				return nullptr;
			}
		}

	private:
		// list of data objects relevant to the corresponding engine
		std::vector<DataObject *> data_objects_;
	};
}
