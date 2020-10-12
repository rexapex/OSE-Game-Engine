#pragma once

namespace ose
{
	struct TagSettings
	{
		struct TagNode
		{
			std::string name_;
			std::vector<TagNode> sub_nodes_;

			TagNode(std::string_view name) : name_(name) {}
		};

		TagNode root_node_ { "" };
	};
}
