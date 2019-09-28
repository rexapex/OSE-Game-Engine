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
		
		// Default copy/move constructors
		///MeshFilter(MeshFilter const &) noexcept = default;
		///MeshFilter & operator=(MeshFilter const &) noexcept = default;
		MeshFilter(MeshFilter &&) noexcept = default;
		MeshFilter & operator=(MeshFilter &&) noexcept = default;
		
		// Copy constructor
		// NOTE - No default copy constructors when std::string used as member
		MeshFilter(const MeshFilter & other) noexcept : Component(other), path_(other.path_) {}

		// Copy assignment constructor
		// NOTE - No default copy constructors when std::string used as member
		MeshFilter & operator=(const MeshFilter & other) noexcept
		{
			Component::operator=(other);
			path_ = other.path_;
			return *this;
		}
	};
}
