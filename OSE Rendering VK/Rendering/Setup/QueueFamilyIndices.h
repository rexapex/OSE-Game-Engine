#pragma once

namespace ose::rendering
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphics_family_;
		std::optional<uint32_t> present_family_;

		bool IsComplete()
		{
			return graphics_family_.has_value() && present_family_.has_value();
		}
	};
}
