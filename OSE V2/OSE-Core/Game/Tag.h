#pragma once

namespace ose
{
	class Tag
	{
	public:
		Tag(std::string const & name);
		~Tag();
		Tag(Tag && other);
		Tag & operator=(Tag && other);

		std::vector<Tag> & GetSubTags() { return sub_tags_; }

	private:
		std::string name_;
		std::vector<Tag> sub_tags_;
	};
}
