#include "stdafx.h"
#include "Texture.h"

namespace origami_sheep_engine
{
	Texture::Texture(const std::string & name, const std::string & path) : name_(name), path_(path) {}

	Texture::~Texture()
	{
		//this object did not allocated memory for img_data_, therefore, this object will not free it
	}
}
