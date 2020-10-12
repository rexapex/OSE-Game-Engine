#include "stdafx.h"
#include "TagManager.h"
#include "TagSettings.h"

namespace ose
{
	Tag TagManager::GetTag(std::string_view name) const
	{
		Tag tag = static_cast<Tag>(0);
		std::function<bool(TagNode const &)> parse_node = [this, &parse_node, &tag, name](TagNode const & node) -> bool {
			if(node.name_ == name)
			{
				tag = node.id_;
				return true;
			}

			for(auto const & n : node.sub_nodes_)
			{
				if(parse_node(n))
					return true;
			}

			return false;
		};

		parse_node(root_node_);
		return tag;
	}

	void TagManager::ApplyTagSettings(TagSettings const & settings)
	{
		std::function<void(TagSettings::TagNode const &, Tag)> parse_node = [this, &parse_node](TagSettings::TagNode const & node, Tag parent) {
			Tag t = AddTag(node.name_, parent);
			for(auto const & n : node.sub_nodes_)
			{
				parse_node(n, t);
			}
		};
		parse_node(settings.root_node_, static_cast<Tag>(0));
	}

	Tag TagManager::AddTag(std::string name, Tag parent)
	{
		if(num_tags_ == max_num_tags_)
			throw std::exception("Failed to add tag, maximum number of tags reached");

		Tag new_tag;

		std::function<bool(TagNode &)> parse_node = [this, &parse_node, &name, parent, &new_tag](TagNode & node) -> bool {
			if(node.id_ == parent)
			{
				new_tag = node.id_ | NextRootTag();
				node.sub_nodes_.emplace_back(new_tag, name);
				return true;
			}

			for(auto & n : node.sub_nodes_)
			{
				if(parse_node(n))
					return true;
			}

			return false;
		};

		parse_node(root_node_);
		return new_tag;
	}
}
