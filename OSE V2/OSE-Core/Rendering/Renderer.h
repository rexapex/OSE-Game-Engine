#pragma once

#include "../../stdafx.h"

namespace origami_sheep_engine
{
	struct Renderer
	{
		Renderer(const std::string & name) : name_(name) {}
		std::string name_;
	};
}