#pragma once

#include "../../stdafx.h"

namespace origami_sheep_engine
{
	class Tag
	{
	public:
		Tag(const std::string & name);
		~Tag();
		Tag(Tag && other);
		Tag & operator=(Tag && other);

		std::vector<Tag> & get_sub_tags() { return sub_tags_; }

	private:
		std::string name_;
		std::vector<Tag> sub_tags_;
	};
}
