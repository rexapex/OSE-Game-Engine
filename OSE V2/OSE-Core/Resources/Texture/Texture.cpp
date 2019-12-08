#include "stdafx.h"
#include "Texture.h"

namespace ose::resources
{
	Texture::Texture(const std::string & name, const std::string & path) : name_(name), path_(path) {}

	Texture::~Texture()
	{
		// this object did not allocate memory for img_data_, therefore, this object will not free it
	}
}
