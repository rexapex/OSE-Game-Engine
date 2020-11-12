#include "stdafx.h"
#include "Texture.h"

namespace ose
{
	Texture::Texture(std::string const & name, std::string const & path) : name_(name), path_(path) {}

	Texture::~Texture()
	{
		// this object did not allocate memory for img_data_, therefore, this object will not free it
	}
}
