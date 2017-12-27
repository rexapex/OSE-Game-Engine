#pragma once
#include "stdafx.h"

struct Component
{
	//fields shared by all component types
	std::string name;

	//constructor
	Component(const std::string & n) : name(n) {}

	//allow sub classes to implement destructors
	virtual ~Component() {}

	//TODO - implement copy constructor and move constructor
	//Component(const Component &) = delete;
	//const Component & operator=(const Component &) = delete;
	//Component(const Component &&) = delete;
	//const Component & operator=(const Component &&) = delete;
};
