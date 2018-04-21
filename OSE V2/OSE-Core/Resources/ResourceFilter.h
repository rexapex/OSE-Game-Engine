#pragma once

namespace ose::resources
{
	class ResourceFilter
	{
	private:
		//component name
		std::string name_;

	public:

		//allocate rendering engine data
		ResourceFilter(const std::string & name) : name_(name) {}

		//de-allocate rendering engine data
		virtual ~ResourceFilter() {}
		
		//copy constructor
		ResourceFilter(const ResourceFilter & other) noexcept : name_(other.name_) {}

		//copy assignment constructor
		ResourceFilter & operator=(const ResourceFilter & other) noexcept
		{
			name_ = other.name_;
			return *this;
		}

		//move constructor
		ResourceFilter(const ResourceFilter && other) noexcept : name_(std::move(other.name_)) {}

		//move assignment constructor
		ResourceFilter & operator=(const ResourceFilter && other) noexcept
		{
			name_ = std::move(other.name_);
			return *this;
		}
	};
}
