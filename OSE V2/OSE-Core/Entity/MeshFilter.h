#pragma once
#include "Component.h"

namespace ose::entity
{
	using namespace resources;

	class MeshFilter : public Component
	{
	// declare MeshFilter as an OSE Component
	COMPONENT(MeshFilter, Component)

	private:
		//data
		std::string path_;	//get rid of this when models are implemented

	public:
		//allocate rendering engine data
		MeshFilter(const std::string & name, const std::string & path) : Component(name), path_(path) {}

		//de-allocate rendering engine data
		virtual ~MeshFilter() {}
		
		//copy constructor
		MeshFilter(const MeshFilter & other) noexcept : Component(other), path_(other.path_) {}

		//copy assignment constructor
		MeshFilter & operator=(const MeshFilter & other) noexcept
		{
			Component::operator=(other);
			path_ = other.path_;
			return *this;
		}

		//move constructor
		MeshFilter(const MeshFilter && other) noexcept : Component(std::move(other)), path_(std::move(other.path_)) {}

		//move assignment constructor
		MeshFilter & operator=(const MeshFilter && other) noexcept
		{
			Component::operator=(std::move(other));
			path_ = std::move(other.path_);
			return *this;
		}
	};
}
