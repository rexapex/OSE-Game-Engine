#pragma once
#include "../Resources/ResourceFilter.h"

namespace ose::resources
{
	class MeshFilter : public ResourceFilter
	{
	private:
		//data
		std::string path;	//get rid of this when models are implemented

	public:
		//allocate rendering engine data
		MeshFilter(const std::string & name, const std::string & path) : ResourceFilter(name), path(path) {}

		//de-allocate rendering engine data
		virtual ~MeshFilter() {}
		
		//copy constructor
		MeshFilter(const MeshFilter & other) noexcept : ResourceFilter(other), path(other.path) {}

		//copy assignment constructor
		MeshFilter & operator=(const MeshFilter & other) noexcept
		{
			ResourceFilter::operator=(other);
			path = other.path;
			return *this;
		}

		//move constructor
		MeshFilter(const MeshFilter && other) noexcept : ResourceFilter(other), path(std::move(other.path)) {}

		//move assignment constructor
		MeshFilter & operator=(const MeshFilter && other) noexcept
		{
			ResourceFilter::operator=(other);
			path = std::move(other.path);
			return *this;
		}

		//clone method which can be overwritten by base classes
		/*virtual std::unique_ptr<Component> clone() const
		{
			return std::make_unique<MeshFilter>(*this);
		}*/
	};
}
