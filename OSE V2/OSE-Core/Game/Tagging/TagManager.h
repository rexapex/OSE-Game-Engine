#pragma once

#include "Tag.h"

namespace ose
{
	struct TagSettings;

	class TagManager
	{
	public:
		TagManager() {}
		virtual ~TagManager() {}
		TagManager(TagManager &) = delete;
		TagManager(TagManager && other) noexcept = default;
		TagManager & operator=(TagManager &) = delete;
		TagManager & operator=(TagManager &&) noexcept = delete;

		Tag GetTag(std::string_view name) const;

		void ApplyTagSettings(TagSettings const & settings);

	private:
		struct TagNode
		{
			Tag id_;
			std::string name_;
			std::vector<TagNode> sub_nodes_;

			TagNode(Tag id, std::string_view name) : id_(id), name_(name), sub_nodes_() {}
		};

		TagNode root_node_ { 0, "" };

		int num_tags_ { 0 };
		int max_num_tags_ { 64 };
		int next_root_tag_ { 1 };

		Tag AddTag(std::string name, Tag parent);

		Tag NextRootTag()
		{
			uint64_t next = next_root_tag_;
			next_root_tag_ = next_root_tag_ << 1;
			return static_cast<Tag>(next);
		}
	};
}
