#pragma once

#include "../../stdafx.h"

namespace ose::rendering
{
	struct Renderer
	{
		Renderer(const std::string & name) : name_(name) {}
		std::string name_;
	};
}