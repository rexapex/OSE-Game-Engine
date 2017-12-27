#pragma once
#include "stdafx.h"
#include "Component.h"

struct MeshFilter : public Component
{
	std::string path;
	//rendering engine data

	//allocate rendering engine data
	MeshFilter(const std::string & name, const std::string & path) : Component(name), path(path) {}

	//de-allocate rendering engine data
	~MeshFilter() {}
};
