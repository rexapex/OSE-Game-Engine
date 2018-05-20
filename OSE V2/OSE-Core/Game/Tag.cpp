#include "stdafx.h"
#include "Tag.h"

namespace ose::game
{
	Tag::Tag(const std::string & name) : name_(name)
	{

	}


	Tag::~Tag()
	{

	}


	Tag::Tag(Tag && other)
	{
		name_ = std::move(other.name_);
		sub_tags_ = std::move(other.sub_tags_);
	}


	Tag & Tag::operator=(Tag && other)
	{
		name_ = std::move(other.name_);
		sub_tags_ = std::move(other.sub_tags_);
		return *this;
	}
}
