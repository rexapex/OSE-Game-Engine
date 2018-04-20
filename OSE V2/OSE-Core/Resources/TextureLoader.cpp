#pragma once

#include "../../stdafx.h"
#include "TextureLoader.h"

namespace origami_sheep_engine
{
	TextureLoader::TextureLoader(const std::string & project_path) : project_path_(project_path)
	{

	}

	TextureLoader::~TextureLoader()	//free all texture resources
	{

	}
}
