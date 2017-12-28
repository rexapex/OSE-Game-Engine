#pragma once
#include "stdafx.h"
#include "Component.h"

struct MeshFilter : public Component
{
	//data
	std::string path;



	//allocate rendering engine data
	MeshFilter(const std::string & name, const std::string & path) : Component(name), path(path) {}

	//de-allocate rendering engine data
	virtual ~MeshFilter() {}



	//copy constructor
	MeshFilter(const MeshFilter & other) noexcept : Component(other), path(other.path) {}

	//copy assignment constructor
	MeshFilter & operator=(const MeshFilter & other) noexcept
	{
		Component::operator=(other);
		path = other.path;
		return *this;
	}

	//move constructor
	MeshFilter(const MeshFilter && other) noexcept : Component(other), path(std::move(other.path)) {}

	//move assignment constructor
	MeshFilter & operator=(const MeshFilter && other) noexcept
	{
		Component::operator=(other);
		path = std::move(other.path);
		return *this;
	}

	//clone method which can be overwritten by base classes
	virtual std::unique_ptr<Component> clone() const
	{
		return std::make_unique<MeshFilter>(*this);
	}
};
