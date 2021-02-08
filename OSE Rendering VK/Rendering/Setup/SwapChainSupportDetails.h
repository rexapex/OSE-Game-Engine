#pragma once

namespace ose::rendering
{
	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities_;
		std::vector<VkSurfaceFormatKHR> formats_;
		std::vector<VkPresentModeKHR> present_modes_;
	};
}
